#pragma once

#include <functional>
#include <unistd.h>

inline bool forkTry(std::function<void ()> &&func) {
    int pipefd[2];
    bool result;

    pipe(pipefd);

    if (fork()) {
        // parent process

        read(pipefd[0], &result, sizeof(result));

        return result;
    } else {
        // child process

        try {
            func();

            result = true;
        } catch (...) {
            result = false;
        }

        write(pipefd[1], &result, sizeof(result));
        exit(0);
    }
}
