#ifndef SPEAKER_H
#define SPEAKER_H

#define PIT_FREQUENCY 1193180

void speaker_enable();
void speaker_disable();
void set_frequency(unsigned short frequency);

#endif