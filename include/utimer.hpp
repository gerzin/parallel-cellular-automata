#ifndef PARALLEL_CELLULAR_AUTOMATA_UTIMER_HPP
#define PARALLEL_CELLULAR_AUTOMATA_UTIMER_HPP

#include <chrono>
#include <string>

/**
 * @brief RAII Timer that prints on cout the time elapsed since its creation and a message.
 * The time is measured in microseconds.
 *
 */
class UTimer
{
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point stop;
    std::string message;
    using usecs = std::chrono::microseconds;
    using msecs = std::chrono::milliseconds;

  protected:
    long *us_elapsed;

  public:
    /**
     * @brief Construct a new UTimer object.
     *
     * @param m Message describing what is being computed.
     *
     */
    UTimer(const std::string m);
    /**
     * @brief Construct a new UTimer object. When the object is destroied inserts in *us
     *        the microseconds elapsed since the creation.
     *
     * @param m Message describing what is being computed.
     * @param us Pointer to variable in which to put the elapsed microsecond.
     *
     */
    UTimer(const std::string m, long *us);
    /**
     * @brief Destroy the UTimer object and print the message containing the elapsed time.
     *
     */
    ~UTimer();
};

#endif