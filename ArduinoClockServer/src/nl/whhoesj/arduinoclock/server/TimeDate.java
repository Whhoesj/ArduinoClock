package nl.whhoesj.arduinoclock.server;

public class TimeDate {

    private int hour, minute, second, day, month, year;
    private int mode;

    public static final int MODE_TIME = 1;
    public static final int MODE_DATE = 2;

    public TimeDate(int mode) {
        this.mode = mode;
    }

    public void set(int i1, int i2, int i3) {
        if (mode == MODE_TIME) {
            this.hour = i1;
            this.minute = i2;
            this.second = i3;
        } else {
            this.day = i1;
            this.month = i2;
            this.year = i3;
        }
    }

    public int getMode() { return mode; }

    public int getHour() {
        return hour;
    }

    public int getMinute() {
        return minute;
    }

    public int getSecond() {
        return second;
    }

    public int getDay() {
        return day;
    }

    public int getMonth() {
        return month;
    }

    public int getYear() {
        return year;
    }

    public String toString() {
        String output = "";
        if (mode == MODE_TIME) {
            if (hour < 10) output += 0;
            output += hour;
            output += ":";
            if (minute < 10) output += 0;
            output += minute;
            output += ":";
            if (second < 10) output += 0;
            output += second;
            return output;
        } else {
            output += day + " - " + month + " - " + year;
            return output;
        }
    }
}
