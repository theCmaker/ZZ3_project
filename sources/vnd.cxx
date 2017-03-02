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

template <unsigned max_itr>
VND<max_itr>::VND() :
	_list({
		  new MoveInsert<BestAvailablePolicy>,
		  new MoveMove2Routes<BestAvailablePolicy>,
		  new MoveSwap1Route<FirstAvailablePolicy>,
		  new Move2Opt<BestAvailablePolicy>,
		  new MoveExtract<BestAvailablePolicy>,
		  new MoveMove1Route<BestAvailablePolicy>,
		  new MoveSwap2Routes<BestAvailablePolicy>,
		  new MoveReplace<FirstAvailablePolicy>
		  }),
	_donotclean(false)
{}

template <unsigned max_itr>
VND<max_itr>::VND(std::vector<Move *> & list) :
	_list(list),
	_donotclean(true)
{}

template <unsigned max_itr>
VND<max_itr>::~VND()
{
	if (!_donotclean) {
		for (std::vector<Move *>::iterator itr = _list.begin(); itr != _list.end(); ++itr) {
			delete *itr;
		}
		_list.clear();
	}
}

template <unsigned max_itr>
void VND<max_itr>::run(Solution & sol) {
	(*this)(sol);
}

template <unsigned max_itr>
void VND<max_itr>::operator() (Solution & sol) {
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

template <unsigned max_itr>
std::vector<Move *> & VND<max_itr>::movements()
{
	return _list;
}

template <unsigned max_itr>
void VND<max_itr>::before(const Solution & s)
{
	AvailablePolicy::maxAcceptableTime() = 1.1 * s.worstInterceptionTime();
	AvailablePolicy::minAcceptableCount() = s.problem().nbMobiles() - 1.1 * (s.problem().nbMobiles() - s.totalInterceptionCount());
}

template <unsigned max_itr>
void VND<max_itr>::after(const Solution &)
{

}
