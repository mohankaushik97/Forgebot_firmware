#define Kp 3
#define Ki 0.01
#define Kd 0

using namespace std;

#define PWM_FREQUENCY 5000    // 1 - 50000 Hz
#define PWM_BIT_RESOLUTION 10 // pwm bit resolution

#define MAX_DUTY ((1 << PWM_BIT_RESOLUTION) - 1)

#define FORWARD 1
#define REVERSE -1

#define TOLERANCE 0

#define PULSE_PER_ROTATION 748
