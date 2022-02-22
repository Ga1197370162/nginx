#include <iostream>

#include "global.h"
#include "ngx_c_log.h"
using namespace myskill;

ngx_c_log* log = ngx_c_log::GetInstance();

int main() {
	NGX_LOG << "helo" << 12 << std::endl;

	std::cin.get();
	return 0;
}