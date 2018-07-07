/*
 * silent-32bit-overflow.c v1.0
 *
 * Demonstration of simple C code where a variable holds a duration value
 * which is then increased in resolution but without due consideration
 * for likely maximum values
 *
 * Detailed discsussion on TODO URL HERE !!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 * cf assembler access to flags and __builtin_umull_overflow()
 * */

#include <stdio.h>
#include <stdint.h>  /* C99 standard */

/* Two typedefs here but a typical code modification would be to
 * upgrade Duration from an int to something else
 *
 * cf LLP64 and LP64
 * */

typedef int Duration;
typedef uint32_t OtherDuration;
typedef uint64_t ReplacementDuration;
#define ReplacementDurationFormat "%lu"

void demo1() {
    int i = 0;
    int timer_us = 0;

    printf("%s\n", __func__);
    for (i=0; i < 5; i++) {
        timer_us += 500*1000;  /* add half a second */
        printf("timer_us=%d\n", timer_us);
    } 
}

void demo2() {
    int i = 0;
    Duration timer_ns = 0;

    printf("%s\n", __func__);
    for (i=0; i < 5; i++) {
        timer_ns += 500000000000;  /* add half a second */
        printf("timer_us=%d timer_us=%ld\n", timer_ns, timer_ns);
    } 
}

void demo3() {
    int i = 0;
    Duration timer_ns = 0;
    int six_us = 6e6;
    int six_ns = six_us * 1000; /* no compiler warning - static tools may detect this */
    /* gcc 4.8.5 warning: overflow in implicit constant conversion [-Woverflow] */
    /* int six_ns = 6000000000000; */

    printf("%s\n", __func__);
    timer_ns += six_ns;
    printf("timer_ns=%ld\n", timer_ns, timer_ns);
}

void demo4() {
    int i = 0;
    ReplacementDuration timer_ns = 0;
    int six_us = 6e6;
    int64_t six_ns = six_us * 1000;  /* looks reasonable ... */

    printf("%s\n", __func__);
    timer_ns += six_ns;
    printf("timer_ns=%ld\n", timer_ns, timer_ns);
}

void demo5() {
    int i = 0;
    ReplacementDuration timer_ns = 0;
    int six_us = 6e6;
    int64_t six_ns = six_us * 1000L;  /* note L suffix (LLP64??) */

    printf("%s\n", __func__);
    timer_ns += six_ns;
    printf("timer_ns=%ld\n", timer_ns, timer_ns);
}

void demo6() {
    int i = 0;
    ReplacementDuration timer_ns = 0;

    printf("%s\n", __func__);
    for (i=0; i < 5; i++) {
        timer_ns += 500000000000L; /* add half a second */
        printf("timer_ns=%ld\n", timer_ns);
    } 
}

void demo7() {
    int i = 0;
    ReplacementDuration timer_ns = 0;
    OtherDuration localtimer = 0;
    int rarecondition = 1;

    printf("%s\n", __func__);
    timer_ns = 500000000000L;  /* set to half a second */
    localtimer += 2*1000*1000*1000; /* two seconds */
    localtimer += 2*1000*1000*1000; /* two more seconds */
    if (rarecondition) { 
        localtimer += 1*1000*1000*1000; /* just one more second */
    }
    printf("timer_ns=%u\n", localtimer);
    timer_ns += localtimer;  /* add half a second */
    printf("timer_ns=" ReplacementDurationFormat "\n", timer_ns);
}


int demo8() {
  int about_to_terminate = 0100;  /* is this 100 ? */
  int could_be_any_value = 0;

  return about_to_terminate / could_be_any_value;
}


int main(int argc, char *argv[]) {
    demo1();
    demo2();
    demo3();
    demo4();
    demo5();
    demo6();
    demo7();
    demo8();
}
