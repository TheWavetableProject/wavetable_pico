#include <stdio.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/binary_info.h"
#include "pico/time.h"

#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "main.pio.h"

uint MICROSTEP = 32; // microstep determined by dip switches. 1, 2, 3 all UP means 32
const uint STEPS_PER_ROTATION = 200;
const double BELT_RATIO = 100./7.;        // TODO check value: diameter hub / diameter stepper

const float RPM_MAX = 30;       // anything above this stalled the Kysan 1124090 stepper used for testing
const float RPM_MIN = 0.01;

#define BUILTIN_LED_PIN 25      // IMPROVE: there is a default provided define for this number
const uint OUTPUT_PIN = 28;     // 7th pin from the top right

const uint FLASH_PERIOD = 1e2;  // ms
const int CORECOM_FLASH = 1;    // IMPROVE: enums are for nerds
const int CORECOM_SOLID = 2;
const int CORECOM_ERROR = -1;

// current state
float sine_amplitude = 0;   // rpm
float sine_frequency = 1;   // rpm
float target_rpm = 1;                                         // INITIAL RPM
float interp_rpm = 1;
float actual_rpm = 0;       // true rpm of the physical device due to PIO timing limitations
bool  stablized = false;    // start stablized at false so that the stepper starts before input TODO: is this wanted?

void print_progbar(const uint len, const double lo, const double hi, const double pos, const double base_pos)
{
    printf("%.2lf |", lo);
    for (uint i=0; i<len; ++i) {
        if ((double)i/len <= pos && pos <= (double)(i+1)/len)
            printf("o");
        else if ((double)i/len <= base_pos)
            printf("-");
        else
            printf(" ");
    }
    printf("| %.2lf ", hi);
}

uint elapsed() {
    return to_ms_since_boot(get_absolute_time());
}

void core1_entry()
{
    char buf[64];
    float arg;
    int comm_state = CORECOM_SOLID;
// main loop
    while (1) {
    // input
        memset(buf, 0, sizeof buf); arg = 0;
        scanf("%63s", buf);  // IMPROVEMENT: async scanf so that the blinking actually works

        if       (!strcmp(buf, "set")) {                                                // soft set RPM
            scanf("%f", &arg);
            if      (arg < RPM_MIN) printf("\nERR: Target %.3f rpm too low!\n",  arg);  // TODO: consider sine-ness
            else if (arg > RPM_MAX) printf("\nERR: Target %.3f rpm too high!\n", arg);
            else multicore_fifo_push_blocking(*(uint*)&arg);
        }
        else if (!strcmp(buf, "fset")) {                                                // force set RPM (no rampup)
            scanf("%f", &arg);
            printf("\nFORCIBLY SETTING RPM... THIS MAY STALL YOUR MOTOR\n");
            if      (arg < RPM_MIN) printf("\nERR: Target %.3f rpm too low!\n",  arg);
            else if (arg > RPM_MAX) printf("\nERR: Target %.2f rpm too high!\n", arg);
            else multicore_fifo_push_blocking(-2),                                  
                 multicore_fifo_push_blocking(*(uint*)&arg);
        }
        else if (!strcmp(buf, "microstep")) {                                           // set microstep ratio
            int new_mstep; scanf("%d", &new_mstep);
            if (new_mstep < 0) printf("\nERR: Microstep ratio %d invalid!\n", new_mstep);
            else printf("\nSuccessfully set microstep ratio to %u\n", (uint)new_mstep),
                 MICROSTEP = (uint) new_mstep,
                 multicore_fifo_push_blocking(~1+1);
        }
        else if (!strcmp(buf, "sina")) {                                                // force set RPM (no rampup)
            scanf("%f", &arg);
            if      (target_rpm - arg < RPM_MIN) printf("\nERR: Amplitude %.2f would lead to min %.2f rpm, which is too low!\n", arg, target_rpm-arg);
            else if (target_rpm + arg > RPM_MAX) printf("\nERR: Amplitude %.2f would lead to max %.2f rpm, which is too high!\n", arg, target_rpm+arg);
            else multicore_fifo_push_blocking(-3),                                  
                 multicore_fifo_push_blocking(*(uint*)&arg);
        }
        else if (!strcmp(buf, "sinf")) {                                                // force set RPM (no rampup)
            scanf("%f", &arg);
            if      (arg < 0) printf("\nERR: Target %.3f frequency invalid!\n",  arg);
            else multicore_fifo_push_blocking(-4),                                  
                 multicore_fifo_push_blocking(*(uint*)&arg);
        }
        else if (!strcmp(buf, "info")) {                                                // print debug info
            printf("\n\nSOFTWARE CONFIGURATION INFO:\nCode commit: <COMMIT]+      \n\nHub dia. / Stepper dia.     %f\nMicrostep                   %u\n\nCore clock speed            %f MHz\nState machine (SM) divisor  %u\nSM clock speed              %f MHz\n\n", BELT_RATIO, MICROSTEP, pio_clock_freq()/1e6, bitbang0_clock_divisor, pio_clock_freq() / 1e6 / bitbang0_clock_divisor);
        }
        else printf("\nUnrecognized command '%s'\n", buf);

    // status communication
        if (multicore_fifo_rvalid())
            comm_state = multicore_fifo_pop_blocking();
        if (comm_state == CORECOM_FLASH) {
            gpio_put(BUILTIN_LED_PIN, 0);
            sleep_ms(FLASH_PERIOD);
        } else {
            printf("\n");
        }
        gpio_put(BUILTIN_LED_PIN, 1);
        sleep_ms(FLASH_PERIOD);
    }
}

float get_sine_amplitude(float sine_amplitude, float sine_frequency, uint operation_timestamp)
{
    const double x_pos = (elapsed()-operation_timestamp)/1e3 /60;
    printf("offset %.3f loc %.3f ", x_pos, x_pos * sine_frequency);
    return sine_amplitude*sin(x_pos*2.*M_PI*sine_frequency);
}

double CLOCK_FREQ = -1;
const double DRIVE_FACTOR = (double)STEPS_PER_ROTATION*BELT_RATIO;
double calc_delay_time(const double rpm)
{
	return 30./rpm / MICROSTEP/DRIVE_FACTOR;
}
double set_rpm(const PIO pio, const uint sm, const double rpm)
{
    if (CLOCK_FREQ < 0) CLOCK_FREQ = (double)pio_clock_freq()/(double)bitbang0_clock_divisor;
    double ans = calc_delay_time(rpm)*CLOCK_FREQ;
    if (ans < bitbang0_upkeep_instruction_count
            || CLOCK_FREQ/(unsigned)ans/2 > 6e5) return -1; // too fast, not possible
    if (ans > 1e7/bitbang0_clock_divisor) return -1;        // too slow, disallow IMPROVE: change condition
    const double reached = (double)30/((unsigned)ans)*CLOCK_FREQ/MICROSTEP/DRIVE_FACTOR;
    printf(" (%uipt %.2lfHz %.2lfrpm)                                  \r", (unsigned)ans, CLOCK_FREQ/(unsigned)ans/2, reached);
    //if (pio_sm_is_tx_fifo_full(pio, sm)) printf("\n\n\nFIFO FULL! %u\n\n\n", elapsed());
    pio_sm_put_blocking(pio, sm, (unsigned)ans-bitbang0_upkeep_instruction_count);
    return reached;
}
int core0_entry(PIO pio, int sm)
{
    // implementation specific
    bool is_force_set = false;

    uint prev_timestamp = elapsed();
    uint operation_timestamp = elapsed();

    float* to_update = &target_rpm;
// main loop
    while (1) {
        if (multicore_fifo_rvalid()) {
            uint g = multicore_fifo_pop_blocking();
            float val = *(float*)&g;
            bool is_nop = 0;    // IMPROVE: scuffed as heck

            if (g>>31&1) { // control signal
                switch (*(int*)&g) {
                case -1: to_update = &target_rpm; is_nop = 1; break;
                case -2: is_force_set = true; to_update = &target_rpm; break;
                case -3: to_update = &sine_amplitude; break;
                case -4: to_update = &sine_frequency; break;
                }
                if (! is_nop) {
                    g = multicore_fifo_pop_blocking(); // IMPROVE: this should check if read is valid, and set a flag to remember the read state between iters
                    val = *(float*)&g;
                }
            } else {    // default to updating the base RPM
                to_update = &target_rpm;
            }
            if (! is_nop) *to_update = val;
            operation_timestamp = elapsed();
            stablized = false;

            // impl specific logic
            if (is_force_set) is_force_set = false, interp_rpm = target_rpm;

            multicore_fifo_push_blocking(CORECOM_FLASH);
        }

        // ramp to target base RPM 
        if (!stablized) {
            //float delta = log(1+interp_rpm) * (elapsed()-prev_timestamp)/100; // IMPROVE: what's the best ramp function? use cosine interp?
            float delta = fmax((float)(elapsed()-prev_timestamp)/1000, 0.0005/sqrt(sqrt(interp_rpm+1)));   // a slower ramp function allows for higher max speed
            if (fabs(target_rpm-interp_rpm) < delta) {
                printf("Best target approximation reached");
                stablized = true;
                interp_rpm = target_rpm;
                operation_timestamp = elapsed();
                multicore_fifo_push_blocking(CORECOM_SOLID);
            } else {
                if (target_rpm > interp_rpm) interp_rpm += delta;
                else                         interp_rpm -= delta;
                printf("Interpolating to %.3f rpm", target_rpm);
            }
            actual_rpm = set_rpm(pio, sm, interp_rpm);
        } else {    // stabilized
            if (sine_amplitude > 0) {   // set sine curve
                printf("set rpm to target %.3f ", target_rpm);
                interp_rpm = target_rpm + get_sine_amplitude(sine_amplitude, sine_frequency, operation_timestamp);
                printf("sinout %.3f ", interp_rpm);
                actual_rpm = set_rpm(pio, sm, interp_rpm);
            }
        }

        sleep_ms(fmax(calc_delay_time(actual_rpm)-elapsed()+prev_timestamp-1, 0));  // TODO: fifo still full sometimes/interpolation is choppy
        prev_timestamp = elapsed(); // NOTE: could improve using absolute_time_t delayed_by_ms() 
    }
}

int main() {
// metadata
    bi_decl(bi_program_description("The Wavetable Project Controller"));

// hardware setup
    stdio_init_all();
    gpio_init(BUILTIN_LED_PIN);
    gpio_set_dir(BUILTIN_LED_PIN, GPIO_OUT);
    gpio_put(BUILTIN_LED_PIN, 1);

// pio init
    const PIO pio = pio0;
    const int sm = 0;
    const uint offset = pio_add_program(pio, &bitbang0_program);
    bitbang0_init(pio, sm, offset, OUTPUT_PIN, 1);

// multicore setup
    multicore_launch_core1(core1_entry);
    core0_entry(pio, sm);
}
