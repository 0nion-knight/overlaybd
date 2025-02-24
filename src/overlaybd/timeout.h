/*
   Copyright The Overlaybd Authors

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#pragma once
#include <inttypes.h>

namespace photon {
extern uint64_t now;
}

class Timeout {
public:
    Timeout(uint64_t x) {
        timeout(x);
    }
    uint64_t timeout(uint64_t x) {
        return m_expire = sat_add(photon::now, x);
    }
    uint64_t timeout() const {
        return sat_sub(m_expire, photon::now);
    }
    operator uint64_t() const {
        return timeout();
    }
    uint64_t timeout_us() const {
        return timeout();
    }
    uint64_t timeout_ms() const {
        return divide(timeout(), 1000);
    }
    uint64_t timeout_MS() const {
        return divide(timeout(), 1024);
    } // fast approximation
    uint64_t timeout_s() const {
        return divide(timeout(), 1000 * 1000);
    }
    uint64_t timeout_S() const {
        return divide(timeout(), 1024 * 1024);
    } // fast approximation
    uint64_t expire() const {
        return m_expire;
    }
    uint64_t expire(uint64_t x) {
        return m_expire = x;
    }

protected:
    uint64_t m_expire; // time of expiration, in us

    // Saturating addition, no upward overflow
    __attribute__((always_inline)) static uint64_t sat_add(uint64_t x, uint64_t y) {
        register uint64_t z asm("rax");
        asm("add %2, %1; sbb %0, %0; or %1, %0;" : "=r"(z), "+r"(x) : "r"(y) : "cc");
        return z;
    }

    // Saturating subtract, no downward overflow
    __attribute__((always_inline)) static uint64_t sat_sub(uint64_t x, uint64_t y) {
        return x > y ? x - y : 0;
    }

    static uint64_t divide(uint64_t x, uint64_t divisor) {
        return (x + divisor / 2) / divisor;
    }
};
