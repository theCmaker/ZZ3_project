#include "vnd.hpp"
unsigned VND::max_itr = 30;

/*
 * VND Best Results (extracted from benchmark)
 *
 {
  new MoveMove2Routes<BestAvailablePolicy>,
  new MoveSwap1Route<FirstAvailablePolicy>,
  new Move2Opt<BestAvailablePolicy>,
  new MoveExtract<BestAvailablePolicy>,
  new MoveMove1Route<BestAvailablePolicy>,
  new MoveInsert<BestAvailablePolicy>,
  new MoveSwap2Routes<BestAvailablePolicy>,
  new MoveReplace<FirstAvailablePolicy>
  }

  {
  new MoveSwap1Route<FirstAvailablePolicy>,
  new MoveExtract<BestAvailablePolicy>,
  new MoveMove2Routes<BestAvailablePolicy>,
  new Move2Opt<FirstAvailablePolicy>,
  new MoveReplace<BestAvailablePolicy>,
  new MoveMove1Route<FirstAvailablePolicy>,
  new MoveInsert<BestAvailablePolicy>,
  new MoveSwap2Routes<FirstAvailablePolicy>
  }

 {
  new MoveSwap1Route<FirstAvailablePolicy>,
  new MoveExtract<BestAvailablePolicy>,
  new MoveSwap2Routes<FirstAvailablePolicy>,
  new MoveMove2Routes<BestAvailablePolicy>,
  new MoveReplace<FirstAvailablePolicy>,
  new Move2Opt<FirstAvailablePolicy>,
  new MoveMove1Route<FirstAvailablePolicy>,
  new MoveInsert<FirstAvailablePolicy>
  }
 *
 */
VND::VND() :
	_list({
		  new MoveReplace<FirstAvailablePolicy>,
		  new MoveExtract<BestAvailablePolicy>,
		  new MoveInsert<FirstAvailablePolicy>,
		  new MoveMove2Routes<FirstAvailablePolicy>,
		  new MoveMove1Route<FirstAvailablePolicy>,
		  new MoveSwap1Route<FirstAvailablePolicy>,
		  new MoveSwap2Routes<FirstAvailablePolicy>
		  }),
	_donotclean(false)
{}

VND::VND(std::vector<Move *> & list) :
	_list(list),
	_donotclean(true)
{}


VND::~VND()
{
	if (!_donotclean) {
		for (std::vector<Move *>::iterator itr = _list.begin(); itr != _list.end(); ++itr) {
			delete *itr;
		}
		_list.clear();
	}
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
