#ifndef _PCA9535_
#define _PCA9535_

void Pca9535SetPin(int dev, int pin, int val);
int Pca9535GetPin(int dev, int pin);
void Pca9535Menu(int argc, char *argv[]);

#endif

