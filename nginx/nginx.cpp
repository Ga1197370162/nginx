#include <iostream>
#include "ngx_c_config.h"
using namespace myskill;

int main() {
	ngx_c_config* p = ngx_c_config::GetInstance();

	cin.get();
	return 0;
}