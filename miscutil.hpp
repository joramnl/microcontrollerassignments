#include <cstdio>
#include <cstdint>
#include "mbed.h"

template<typename data_t>
constexpr size_t index(data_t* base, data_t* elem){
    return (elem - base)/sizeof(data_t);
}

namespace sys{
    static DigitalOut status(LED1,LOW);

    class BusyMark final{
        public:
        inline BusyMark(){
            status.write(HIGH);
        }
        inline ~BusyMark(){
            status.write(LOW);
        }
    };
}

namespace bit{
    template<typename raw_t>
    constexpr raw_t mask(int length){
        return (1 << length)-1;
    }

    template<typename raw_t>
    constexpr raw_t select(raw_t raw, int index, int length){
        return (raw >> index) & ::bit::mask<raw_t>(length);
    }
}

namespace print{
	template<int bits>
	inline void binx(int raw){
		binx<bits-1>(raw);
		putchar('0'+ ::bit::select<int>(raw,bits-1,1));
	}
	
	template<>
	inline void binx<0>(int raw){}
	
	namespace {
		char hexchr(char v){
			return v < 10 ? '0'+v : 'A' + v - 10;
		}
	}
	
	void hex(const char * value, size_t length){
		for(const char * const end = value+length;value != end;++value){
			putchar(hexchr(*value >> 4));
			putchar(hexchr(*value&0xf));
		}
	}
	
	void bin(const char * value, size_t length){
		for (const char * const end = value + length;value != end;++value){
			binx<8>(*value);
		}
	}

}

#define elements(a) sizeof(a)/sizeof(a[0])