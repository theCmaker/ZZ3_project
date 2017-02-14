#include "vnd.hpp"
unsigned VND::max_itr = 1000;
VND::VND() : _list({
				   new MoveReplace<FirstAvailablePolicy>,
				   new MoveExtract<BestAvailablePolicy>,
				   new MoveInsert<FirstAvailablePolicy>,
				   new MoveMove2Routes<FirstAvailablePolicy>,
				   new MoveMove1Route<FirstAvailablePolicy>,
				   new MoveSwap1Route<FirstAvailablePolicy>,
				   new MoveSwap2Routes<FirstAvailablePolicy>
				   })
{}

VND::~VND()
{
	for (std::vector<Move *>::iterator itr = _list.begin(); itr != _list.end(); ++itr) {
		delete *itr;
	}
	_list.clear();
}

void VND::run(Solution & sol) {
	unsigned k = 0;
	unsigned itr = 1;
	do {
		if (_list[k]->scan(sol)) {
			_list[k]->commit(sol);
			k = 0;
			++itr;
		} else ++k;

	} while (k < _list.size() && itr < max_itr);
}

std::vector<Move *> & VND::movements()
{
	return _list;
}
