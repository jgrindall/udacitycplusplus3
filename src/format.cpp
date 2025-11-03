#include "format.h"

#include <string>

using std::string;

string paddZero(long number) { return (number < 10 ? "0" : "") + std::to_string(number); }

string Format::ElapsedTime(long seconds) {
  long totalHours = seconds / 3600;
  long minsRemaining = seconds - (totalHours * 3600);
  long totalMinutes = minsRemaining / 60;
  long secsRemaining = minsRemaining - (totalMinutes * 60);
  return paddZero(totalHours) + ":" + paddZero(totalMinutes) + ":" + paddZero(secsRemaining);
}