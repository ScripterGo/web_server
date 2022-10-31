
#pragma once

namespace math {
	int fast_exp(int a, int b) {
		if (b == 0) return 1;
		int q = fast_exp(a, b / 2);
		return b % 2 == 0 ? q * q : q * q * a;
	}

}