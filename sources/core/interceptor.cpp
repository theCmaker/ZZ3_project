#include "interceptor.hpp"
#include "depot.hpp"
#include <cmath>

Interceptor::Interceptor(unsigned id) :
	_id(id),
	_position(0.,0.),
	_speed(0.),
	_range(std::numeric_limits<Time>::infinity()),
	_depot(nullptr)
{}

Interceptor::Interceptor(Location & l, Speed s, unsigned id, Time range) :
	_id(id),
	_position(l),
	_speed(s),
	_range(range),
	_depot(nullptr)
{}

Interceptor::Interceptor(Distance x, Distance y, Speed s, unsigned id, Time range) :
	_id(id),
	_position(x,y),
	_speed(s),
	_range(range),
	_depot(nullptr)
{}

Interceptor::Interceptor(const Depot & d, Speed s, unsigned id, Time range) :
	_id(id),
	_position(d.position()),
	_speed(s),
	_range(range),
	_depot(&d)
{}

Interceptor::~Interceptor() {}

Interceptor::operator unsigned() const
{
	return _id;
}

// Getters
const unsigned & Interceptor::id() const
{
	return _id;
}
const Location & Interceptor::position() const
{
	return _position;
}

Speed Interceptor::speed() const
{
	return _speed;
}

const Depot * Interceptor::depot() const
{
	return _depot;
}

Time Interceptor::range() const
{
	return _range;
}

// Setters
Interceptor & Interceptor::position(const Location & l)
{
	_position = l;
	return *this;
}

Interceptor & Interceptor::speed(const Speed s)
{
	_speed = s;
	return *this;
}

Interceptor & Interceptor::range(Time range)
{
	_range = range;
	return *this;
}

//Methods

void Interceptor::computePosition(double alpha, Location & pos, Time t) const
{
	pos._x = pos._x + t * std::cos(alpha) * _speed;
	pos._y = pos._y + t * std::sin(alpha) * _speed;
}

double Interceptor::computeAlpha(double a, double b, double c)
{
	double res = 42;
	double pi = M_PI;
	double a2 = a * a;
	double b2 = b * b;
	double c2 = c * c;

	if ((c+a)== 0)
	{
		res = pi;
		if ( b != 0 && (a2+b2) != 0)
		{
			res = -2*atan(a/b);
		}
	}
	else
	{
		if ((-b*sqrt(a2+b2-c2)+a2+a*c+b2) != 0)
		{
			res = 2*atan((b-sqrt(a2+b2-c2))/(a+c));
		}
		if ((b*sqrt(a2+b2-c2)+a2+a*c+b2) != 0)
		{
			res = 2*atan((sqrt(a2+b2-c2)+b)/(a+c));
		}
	}
	return res;
}

Time Interceptor::computeInterception(Location position, const Mobile & m, Time t) const
{
	double alpha;
	/* Variables liees a l'intercepteur */
	Speed v1 = _speed;
	Location l1 = position;

	/* Variables liees au mobile traite */
	Direction v0 = m.direction();
	Location l0 = m.position(t);

	/* Variables liees a l'obtention du temps d'interception */
	Time t1, t2;
	Distance epsilon = 0.0001;
	double tres = std::numeric_limits<Time>::infinity();
	int ind1 = 0; /* Indicateurs pour savoir si l'interception fonctionne */

	/* Tester si les positions ne sont pas egales avant de calculer un angle.
	c'est rare mais ca peut arriver, notamment en faisant des tests sans faire attention
	(mobiles qui convergent) */
	if (l0.equals(l1,epsilon)) {

		tres = 0.; /* temps d'interception nul, on est déjà au bon endroit */

	} else {
		/* Variable liees a l'obtention de l'angle alpha */
		double a = l0._y-l1._y;
		double b = l1._x-l0._x;
		double c = (a*v0._sx+b*v0._sy)/v1;
		alpha = Interceptor::computeAlpha(a,b,c); /* Nombre compris entre -Pi et Pi ou Pi à priori vu la résolution de l'équation */
		if (alpha != 42) /* Code d'erreur (manque d'inspiration) */
		{
			t1 =  -b/(-v0._sx+v1*cos(alpha));
			t2 =  a/(-v0._sy+v1*sin(alpha));

			/* Pour choisir la bonne date, il suffit de prendre celle qui marche et si les deux fonctionnent, on prend la date la plus faible */
			/* On gere d'abord la premiere date */
			if (std::isfinite(t1) && t1 >= 0)
			{
				computePosition(alpha,l1,t1);
				l0 = m.position(t1+t);

				if (l0.equals(l1,epsilon)) /* Fonction d'egalite de deux positions */
				{
					tres = t1;
					ind1 = 1;
				}
			}
			l1 = position;
			/* Puis la 2eme */
			if (std::isfinite(t2) && t2 >= 0)
			{
				computePosition(alpha,l1,t2);
				l0 = m.position(t2+t);

				if (l0.equals(l1,epsilon)) /* Fonction d'egalite de deux positions */
				{
					tres = t2;
					if (ind1 && t2 > t1) /* On regarde si t1 a marche aussi et s'il etait plus faible, on le choisit */
					{
						tres = t1;
					}
				}
			}
		}
	}
	if (std::isfinite(tres) && tres+t+timeFromTo(l0,_depot->position()) > _range) {
		tres = INTERCEPTION_TIME_NO_FUEL;
	}
	return tres;
}

std::ostream & operator << (std::ostream & o, const Interceptor & i) {
	o << "[" << i.id() << " pos(" << i.position()._x << "," << i.position()._y << ");spd(" << i.speed() << ");range(" << i.range() << ")]";
	return o;
}
