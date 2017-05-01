/*
 * VND Best Results (extracted from benchmark)
 *
 * 3 12 7 1 2 0 5 14
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

 * 12 1 3 15 6 10 0 13
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

 * 12 1 13 3 14 15 10 8
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

 * 9 5 8 7 3 12 2 14
  {
  new MoveExtract<FirstAvailablePolicy>,
  new MoveSwap2Routes<BestAvailablePolicy>,
  new MoveInsert<FirstAvailablePolicy>,
  new Move2Opt<BestAvailablePolicy>,
  new MoveMove2Routes<BestAvailablePolicy>,
  new MoveSwap1Route<FirstAvailablePolicy>,
  new MoveMove1Route<BestAvailablePolicy>,
  new MoveReplace<FirstAvailablePolicy>
  }

 * 13 7 1 3 12 0 2 14
  {
  new MoveSwap2Routes<FirstAvailablePolicy>,
  new Move2Opt<BestAvailablePolicy>,
  new MoveExtract<BestAvailablePolicy>,
  new MoveMove2Routes<BestAvailablePolicy>,
  new MoveSwap1Route<FirstAvailablePolicy>,
  new MoveInsert<BestAvailablePolicy>,
  new MoveMove1Route<BestAvailablePolicy>,
  new MoveReplace<FirstAvailablePolicy>
  }

 * 14 3 7 5 8 12 2 9
  {
  new MoveReplace<FirstAvailablePolicy>,
  new MoveMove2Routes<BestAvailablePolicy>,
  new Move2Opt<BestAvailablePolicy>,
  new MoveSwap2Routes<BestAvailablePolicy>,
  new MoveInsert<FirstAvailablePolicy>,
  new MoveSwap1Route<FirstAvailablePolicy>,
  new MoveMove1Route<BestAvailablePolicy>,
  new MoveExtract<FirstAvailablePolicy>
  }

 * 3 13 15 8 12 2 1 6 ++
 {
  new MoveMove2Routes<BestAvailablePolicy>,
  new MoveSwap2Routes<FirstAvailablePolicy>,
  new Move2Opt<FirstAvailablePolicy>,
  new MoveInsert<FirstAvailablePolicy>,
  new MoveSwap1Route<FirstAvailablePolicy>,
  new MoveMove1Route<BestAvailablePolicy>,
  new MoveExtract<BestAvailablePolicy>,
  new MoveReplace<BestAvailablePolicy>
 }

 * 3 12 15 0 2 9 14 13
 {
  new MoveMove2Routes<BestAvailablePolicy>,
  new MoveSwap1Route<FirstAvailablePolicy>,
  new Move2Opt<FirstAvailablePolicy>,
  new MoveInsert<BestAvailablePolicy>,
  new MoveMove1Route<BestAvailablePolicy>,
  new MoveExtract<FirstAvailablePolicy>,
  new MoveReplace<FirstAvailablePolicy>,
  new MoveSwap2Routes<FirstAvailablePolicy>
 }

 * 1 13 3 6 15 12 10 0
 {
  new MoveExtract<BestAvailablePolicy>,
  new MoveSwap2Routes<FirstAvailablePolicy>,
  new MoveMove2Routes<BestAvailablePolicy>,
  new MoveReplace<BestAvailablePolicy>,
  new Move2Opt<FirstAvailablePolicy>,
  new MoveSwap1Route<FirstAvailablePolicy>,
  new MoveMove1Route<FirstAvailablePolicy>,
  new MoveInsert<BestAvailablePolicy>
 }

 * 0 6 15 13 3 4 2 9
 {
  new MoveInsert<BestAvailablePolicy>,
  new MoveReplace<BestAvailablePolicy>,
  new Move2Opt<FirstAvailablePolicy>,
  new MoveSwap2Routes<FirstAvailablePolicy>,
  new MoveMove2Routes<BestAvailablePolicy>,
  new MoveSwap1Route<BestAvailablePolicy>,
  new MoveMove1Route<BestAvailablePolicy>,
  new MoveExtract<FirstAvailablePolicy>
 }
 *
 */

template <unsigned max_itr>
VND<max_itr>::VND() :
	_list({
		  new MoveInsert<BestAvailablePolicy>,
		  new MoveReplace<BestAvailablePolicy>,
		  new Move2Opt<FirstAvailablePolicy>,
		  new MoveSwap2Routes<FirstAvailablePolicy>,
		  new MoveMove2Routes<BestAvailablePolicy>,
		  new MoveSwap1Route<BestAvailablePolicy>,
		  new MoveMove1Route<BestAvailablePolicy>,
		  new MoveExtract<FirstAvailablePolicy>
		  }),
	_donotclean(false)
{}

template <unsigned max_itr>
VND<max_itr>::VND(const std::vector<Move *> & list) :
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
	if (_list.size() > 0) {
		unsigned k = 0;
		unsigned itr = 1;
		do {
			if (_list[k]->scan(sol)) {
//				Solution sol_before = sol;
				_list[k]->commit(sol);
				if (!sol.check()) {
					std::cerr << "Context data:" << std::endl
//							  << "Solution before: " << sol_before
							  << std::endl << "Failed at movement " << k << ". Iteration " << itr << '.' << std::endl;
					abort();
				}
				k = 0;
				++itr;
			} else ++k;

		} while (k < _list.size() && itr < max_itr);
	}
}

template <unsigned max_itr>
std::vector<Move *> & VND<max_itr>::movements()
{
	return _list;
}

template <unsigned max_itr>
void VND<max_itr>::before(const Solution & s)
{
	unsigned count = s.problem().nbMobiles() - 1.1 * (s.problem().nbMobiles() - s.totalInterceptionCount());
	AvailablePolicy::maxAcceptableTime() = 1.1 * s.worstInterceptionTime();
	AvailablePolicy::minAcceptableCount() = (count > 0) ? count : 1u;
}

template <unsigned max_itr>
void VND<max_itr>::after(const Solution &)
{

}
