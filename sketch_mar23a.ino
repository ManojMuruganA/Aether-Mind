#include <math.h>
#include <CircularBuffer.h>

#define SAMPLE_RATE 125
#define BAUD_RATE 115200
#define INPUT_PIN A2  // Analog pin
#define OUTPUT_PIN 13
#define DATA_LENGTH 16

float BPM = 0.0;
bool peak = false;

bool IgnoreReading = false;
bool FirstPulseDetected = false;
unsigned long FirstPulseTime = 0;
unsigned long SecondPulseTime = 0;
unsigned long PulseInterval = 0;

CircularBuffer<unsigned long, 10> buffer;
int data_index = 0;

void setup() {
  Serial.begin(BAUD_RATE);
  delay(1000);
  Serial.println("\nECG BPM Monitor Starting...\n");

  pinMode(INPUT_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
}

void loop() {
  static unsigned long lastTime = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastTime >= (1000 / SAMPLE_RATE)) {
    lastTime = currentTime;

    float sensor_value = analogRead(INPUT_PIN);
    float normalized_value = (sensor_value - 512.0) / 512.0; 
    float signal = ECGFilter(normalized_value);

    peak = Getpeak(signal);
    digitalWrite(OUTPUT_PIN, peak);

    if (peak && !IgnoreReading) {
      if (!FirstPulseDetected) {
        FirstPulseTime = millis();
        FirstPulseDetected = true;
      } else {
        SecondPulseTime = millis();
        PulseInterval = SecondPulseTime - FirstPulseTime;

        if (PulseInterval >= 500 && PulseInterval <= 1500) {  
          buffer.unshift(PulseInterval);
          FirstPulseTime = SecondPulseTime;
        }
      }
      IgnoreReading = true;
    }

    if (!peak) {
      IgnoreReading = false;
    }

    if (buffer.size() >= 5) {  
      unsigned long sumIntervals = 0;
      for (int i = 0; i < buffer.size(); i++) {
        sumIntervals += buffer[i];
      }

      float avgInterval = (float)sumIntervals / buffer.size();

      if (avgInterval > 0) {
        BPM = 60000.0 / avgInterval;

        if (BPM >= 60.0 && BPM <= 100.0) {
          Serial.print("BPM: ");
          Serial.println(BPM, 2);  // Display with 2 decimal places
        }
      }
    }
  }

  delay(10);
}

bool Getpeak(float new_sample) {
  static float data_buffer[DATA_LENGTH];
  static float mean_buffer[DATA_LENGTH];
  static float standard_deviation_buffer[DATA_LENGTH];

  data_buffer[data_index] = new_sample;

  float sum = 0.0, mean = 0.0, standard_deviation = 0.0;
  for (int i = 0; i < DATA_LENGTH; ++i) {
    sum += data_buffer[i];
  }
  mean = sum / DATA_LENGTH;

  for (int i = 0; i < DATA_LENGTH; ++i) {
    standard_deviation += pow(data_buffer[i] - mean, 2);
  }

  mean_buffer[data_index] = mean;
  standard_deviation_buffer[data_index] = sqrt(standard_deviation / DATA_LENGTH);

  peak = (new_sample > mean + 2.0 * standard_deviation);  

  data_index = (data_index + 1) % DATA_LENGTH;

  return peak;
}

float ECGFilter(float input) {
  float output = input;

  {
    static float z1 = 0, z2 = 0;
    float x = output - 0.70682283 * z1 - 0.15621030 * z2;
    output = 0.28064917 * x + 0.56129834 * z1 + 0.28064917 * z2;
    z2 = z1;
    z1 = x;
  }

  {
    static float z1 = 0, z2 = 0;
    float x = output - 0.95028224 * z1 - 0.54073140 * z2;
    output = 1.0 * x + 2.0 * z1 + 1.0 * z2;
    z2 = z1;
    z1 = x;
  }

  return output;
}
