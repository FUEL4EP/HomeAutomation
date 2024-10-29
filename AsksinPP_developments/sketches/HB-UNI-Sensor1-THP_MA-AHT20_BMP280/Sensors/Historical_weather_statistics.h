//---------------------------------------------------------
// Historical_weather_statistics
// 2024-05-27 (C) FUELEP (Creative Commons)
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// You are free to Share & Adapt under the following terms:
// Give Credit, NonCommercial, ShareAlike
// +++
//
// library for extracting historical weather data of the last 365 days from historical average monthly temperatures
// e.g. source URL: https://de.statista.com/statistik/daten/studie/5564/umfrage/monatliche-durchschnittstemperatur-in-deutschland/MAX_SAMPLE_INDEX
// please update the table yValues[14] to the latest on published at that URL
//---------------------------------------------------------
//
// used libraries:
//
// imeLib.h             (https://github.com/PaulStoffregen/Time)
// InterpolationLib.h   (https://github.com/luisllamasbinaburo/Arduino-Interpolation/tree/master)
// math.h
//---------------------------------------------------------

#ifndef _HISTORICAL_WEATHER_STATISTICS_H_
#define _HISTORICAL_WEATHER_STATISTICS_H_

#ifndef SAMPLING_INTERVAL
#define SAMPLING_INTERVAL  240  // sampling intervall of weather sensor measurements
#endif


//#define DEEP_DEBUG // comment out if deep serial monitor debugging is not necessary

#include <AskSinPP.h>
#include <TimeLib.h>           //  https://github.com/PaulStoffregen/Time
#include "InterpolationLib.h"  //  https://github.com/luisllamasbinaburo/Arduino-Interpolation/tree/master
#include <math.h>


const uint32_t SAMPLES_PER_YEAR = 365L*24L*3600L/SAMPLING_INTERVAL;   // number of samples per year
const uint32_t SAMPLES_PER_DAY  = 24L*3600L/SAMPLING_INTERVAL;        // number of samples per day


namespace as {


class historical_weather_statistics {

private:



const int numValues = 14;     // one sample at middle of each month + one sample at beginning of year + one sample at end of year = 12 + 1 + 1 = 14
// average monthly temperature in Germany, Source: https://de.statista.com/statistik/daten/studie/5564/umfrage/monatliche-durchschnittstemperatur-in-deutschland/

                      //B Jan  Feb  Mar   Apr   May   Jun   Jul   Aug   Sep  Oct  Nov  Dec     E     // month of year
double xValues[14] = {  0, 16,  45,  75,  105,  136,  166,  197,  228,  258, 289, 319, 350 , 365};   // day of year index of mid of month
double yValues[14] = { 15, 15,  66,  75,  100,  149,  168,  189,  200,  153, 119,  55,  39,   15};   // average monthly temperatures in Germany scaled by 10: 10 deg C == 100, as of E/Oct 2024; please update to the latest published values
                                                                                                     // source URL: https://de.statista.com/statistik/daten/studie/5564/umfrage/monatliche-durchschnittstemperatur-in-deutschland/
                                                                                                     // please update the table yValues[14] to the latest on published at that UR
                                                                                                     // get day of year from the date and time of compilation

                                                                                                     // alternatively, the average monthly temperatures can be extracted e.g. from
                                                                                                     // https://www.wetterkontor.de/de/wetter/deutschland/monatswerte.asp
                                                                                                     // for a specific city of Germany
                                                                                                     // e.g. Frankfurt: https://www.wetterkontor.de/de/wetter/deutschland/monatswerte-station.asp?id=10637

public:




time_t get_EPOCH_time_in_year_from_date_time(char const *date, char const *mytime) {
    char s_month[5];
    int16_t year;
    tmElements_t t, t_beginning_of_year;
#ifdef DEEP_DEBUG
    DPRINTLN(date);
    DPRINTLN(mytime);
#endif
    int16_t dd;

    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    sscanf(date, "%s %d %d", s_month, &dd, &year);
    t.Day = dd;

    int16_t hh, mm, ss;

    sscanf(mytime, "%d:%d:%d", &hh, &mm, &ss);

    t.Hour = hh;
    t.Minute  = mm;
    t.Second  = ss;

    // Find where is s_month in month_names. Deduce month value.
    t.Month = (int8_t)((strstr(month_names, s_month) - month_names) / 3 + 1);

    t.Year = year - 1970;
#ifdef DEEP_DEBUG
    DDECLN(t.Year);
    DDECLN(t.Month);
    DDECLN(t.Day);
    DDECLN(t.Hour);
    DDECLN(t.Minute);
    DDECLN(t.Second);
#endif

// set t_beginning_of_year as beginning of the year of compilatioon
    t_beginning_of_year.Year    = year - 1970;
    t_beginning_of_year.Month   = 1;
    t_beginning_of_year.Day     = 1;
    t_beginning_of_year.Hour    = 0;
    t_beginning_of_year.Minute  = 0;
    t_beginning_of_year.Second  = 0;

#ifdef DEEP_DEBUG
    DPRINT(F("EPOCH time at compilation was ")); DDECLN(makeTime(t));

    DDECLN(t_beginning_of_year.Year);
    DDECLN(t_beginning_of_year.Month);
    DDECLN(t_beginning_of_year.Day);
    DDECLN(t_beginning_of_year.Hour);
    DDECLN(t_beginning_of_year.Minute);
    DDECLN(t_beginning_of_year.Second);

    DPRINT(F("EPOCH time at beginning of year of compilation was ")); DDECLN(makeTime(t_beginning_of_year));

#endif


    return  (makeTime(t) - makeTime(t_beginning_of_year));  // EPOCH time difference since beginning of year of compilation
}

float get_day_fractional_number_of_year_from_date_time(char const *date, char const *mytime) {

    // calculate the fractional day number from the EPOCH time, e.g. 145.345
    return (float)(get_EPOCH_time_in_year_from_date_time(date, mytime)/24/3600);
}

uint16_t get_day_index_of_year_from_date_time(char const *date, char const *mytime) {

    // calculate the integer day index from the EPOCH time
    return (uint16_t)(get_EPOCH_time_in_year_from_date_time(date, mytime)/24/3600);
}


uint32_t get_sampling_index_of_year_from_date_time(char const *date, char const *mytime) {


    uint32_t EPOCH_time_in_year_from_date_time;
    EPOCH_time_in_year_from_date_time = (uint32_t)(get_EPOCH_time_in_year_from_date_time(date, mytime));
    DPRINT(F("EPOCH time in year from date time ")); DDECLN(EPOCH_time_in_year_from_date_time);
    DPRINT(F("SAMPLING_INTERVAL                 ")); DDECLN(SAMPLING_INTERVAL);


    // calculate the integer sampling index from the EPOCH time, i.e. how many measurements samples have been done since beginning of year of compilation
    return (uint32_t)(EPOCH_time_in_year_from_date_time/SAMPLING_INTERVAL);
}


int16_t get_historic_average_temperature_from_statistics_at_day_index(uint16_t day_index_in_year) {
  int16_t avg_temp_i;
  float   avg_temp_f;
  float   xValue;

  if (day_index_in_year <= 365) {
    xValue = (float)day_index_in_year;

    avg_temp_f = Interpolation::CatmullSpline(xValues, yValues, numValues, xValue);
    avg_temp_i = ((int16_t) round(avg_temp_f*10+5))/10;
  }
  else {
    DPRINT(F("ERROR: Wrong day index in year"));
    avg_temp_i = -999;
  }



  return avg_temp_i;
}

int16_t get_historic_average_temperature_from_statistics_at_sample_index(uint32_t sample_index_in_year) {
  int16_t avg_temp_i;
  float   avg_temp_f;
  float   xValue;

  if (sample_index_in_year <= SAMPLES_PER_YEAR) {
    xValue = (float)sample_index_in_year/SAMPLES_PER_DAY;
    #ifdef DEEP_DEBUG
      DPRINT(F("float day index : ")); DDECLN(xValue);
    #endif
    avg_temp_f = Interpolation::CatmullSpline(xValues, yValues, numValues, xValue);
    avg_temp_i = ((int16_t) round(avg_temp_f*10+5))/10;
    #ifdef DEEP_DEBUG
      DPRINT(F("historical average temperature was : ")); DDECLN(avg_temp_i);
    #endif
  }
  else {
    DPRINT(F("ERROR: Wrong day index in year"));
    avg_temp_i = -999;
  }



  return avg_temp_i;
}

void dump_interpolated_daily_temperatures(){
  int16_t avg_temp_i;


  DPRINTLN(F("interpolated daily average temperatures per day index are (scaled by 10.0) :"));
  DPRINTLN(F(""));

  for (uint16_t i = 0; i <= 365; i += 1)
  {

    avg_temp_i = get_historic_average_temperature_from_statistics_at_day_index(i);

    DPRINT(F("day index ")); DDEC(i); DPRINT(F(" : ")); DDEC(avg_temp_i); DPRINTLN(F(" deg C (x10)"));
  }
}

uint16_t get_day_index_in_year_from_date_time_of_compilation() {
  uint16_t day_of_year = get_day_index_of_year_from_date_time( __DATE__,__TIME__);
  DPRINT(F("compilation was done at day ")); DDEC(day_of_year);  DPRINTLN(F(" of the year"));
  return day_of_year;
}

uint32_t get_sample_index_in_year_from_date_time_of_compilation() {
  uint32_t sample_index = get_sampling_index_of_year_from_date_time( __DATE__,__TIME__);
  DPRINT(F("compilation was done at sample index ")); DDEC(sample_index);  DPRINTLN(F(" of the year"));
  return sample_index;
}

};

}

#endif
