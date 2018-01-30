#include "Strategy.h"

class Gang_Strategy : public Strategy {
public:
	Gang_Strategy() {};
	Gang_Strategy(string file);
	~Gang_Strategy() {};
	virtual void init();
};
