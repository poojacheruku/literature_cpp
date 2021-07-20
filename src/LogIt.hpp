#ifndef LITERATURE_LOGGER_HPP
#define LITERATURE_LOGGER_HPP

#include <iostream>
#include <sstream>

/* consider adding boost thread id since we'll want to know whose writting and
 * won't want to repeat it for every single call */

/* consider adding policy class to allow users to redirect logging to specific
 * files via the command line
 */

enum loglevel_e
    {logERROR, logWARNING, logINFO, logDEBUG, logDEBUG1, logDEBUG2, logDEBUG3, logDEBUG4};

class LogIt
{
public:
    LogIt(loglevel_e _loglevel = logERROR) {
        _buffer << _loglevel << " :" 
            << std::string(
                _loglevel > logDEBUG 
                ? (_loglevel - logDEBUG) * 4 
                : 1
                , ' ');
    }

    template <typename T>
    LogIt & operator<<(T const & value)
    {
        _buffer << value;
        return *this;
    }

    ~LogIt()
    {
        _buffer << std::endl;
        // This is atomic according to the POSIX standard
        // http://www.gnu.org/s/libc/manual/html_node/Streams-and-Threads.html
        std::cerr << _buffer.str();
    }

private:
    std::ostringstream _buffer;
};

extern loglevel_e loglevel;

#define logIt(level) \
if (level > loglevel) ; \
else LogIt(level)

#endif  // LITERATURE_LOGGER_HPP
