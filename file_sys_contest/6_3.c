#include <stdio.h>
#include <time.h>
#include <math.h>
enum
{
    BASE_YEAR = 1900,
    GRIG_DAY = 7,
    GRIG_YEAR = 1925,
    GRIG_MONTH = 9,
    DAYS_IN_MON = 30,
    DAYS_IN_YEAR = 360,
    MONTHS_IN_YEAR = 12,
    HOURS_IN_DAY = 24,
    SEC_IN_HOUR = 3600
};

int main(void)
{
    int y, m, d;
    long long mn, dn, yn;
    while (scanf("%d%d%d", &y, &m, &d) == 3) {
        struct tm date1 = {};
        struct tm date2 = {};
        date1.tm_mday = d;
        date1.tm_year = y - BASE_YEAR;
        date1.tm_mon = m - 1;
        date1.tm_isdst = -1;
        date2.tm_mday = GRIG_DAY;
        date2.tm_year = GRIG_YEAR - BASE_YEAR;
        date2.tm_mon = GRIG_MONTH;
        date2.tm_isdst = -1;
        long long first = mktime(&date1);
        long long second = mktime(&date2);
        first -= second;
        dn = round((double)first / (SEC_IN_HOUR * HOURS_IN_DAY));
        mn = dn / DAYS_IN_MON;
        yn = dn / DAYS_IN_YEAR;
        dn -= mn * DAYS_IN_MON;
        mn -= yn * MONTHS_IN_YEAR;
        printf("%lld %lld %lld\n", yn + 1, mn + 1, dn + 1);
    }
    return 0;
}