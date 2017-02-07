#include "vnd.hpp"

VND::VND() : _list({
				   new MoveInsert<FirstAvailablePolicy>
				   //new MoveExtract<BestAvailablePolicy>
				   })
{}

VND::~VND()
{
	for (std::vector<Move *>::iterator itr = _list.begin(); itr != _list.end(); ++itr) {
		delete *itr;
	}
}

void VND::run(Solution & sol) {
	unsigned k = 0;
	do {
		if (_list[k]->scan(sol)) {
			_list[k]->commit(sol);
			k = 0;
		} else ++k;

	} while (k < _list.size());
}
