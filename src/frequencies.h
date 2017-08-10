
#ifndef SRC_FREQUENCIES_H_
#define SRC_FREQUENCIES_H_

#define NUM_BANDS 5
#define NUM_CHANNELS 8

const uint16_t bandMap[][8] = {
        {5865, 5845, 5825, 5805, 5785, 5765, 5745, 5725}, //A band
        {5733, 5752, 5771, 5790, 5809, 5828, 5847, 5866}, //B band
        {5705, 5685, 5665, 5645, 5885, 5905, 5925, 5945}, //E band
        {5740, 5760, 5780, 5800, 5820, 5840, 5860, 5880}, //F band
        {5658, 5695, 5732, 5769, 5806, 5843, 5880, 5917}, //C band (Raceband)
};

const char bandLetters[][2] = {
        "A",
        "B",
        "E",
        "F",
        "C",
};

#endif /* SRC_FREQUENCIES_H_ */
