#pragma once
#include <string>
#include <map>
#include "..\Vector.h"



class ode_t
{
public:
	// Установить настройки модели
	virtual void setSettings(const std::map<std::string, std::string>&) = 0;
	// Установить начальное состояние
	virtual void setNullStateSettings(const std::map<std::string, std::string>&) = 0;
	// Посчитать производную в точке
	virtual void calculateDerivative(double time, const Vector<double>& state, Vector<double>& derivative) const = 0;
	// Получить размерность вектора состояния
	virtual size_t getDimension() const = 0;
	// Получить начальное состояние
	virtual Vector<double> getNullState() const = 0;
	// Может быть не виртуальной (переместить нулевое состояние в базовый класс)
	virtual ~ode_t()
	{ }
};


class LorenceAttractor : public ode_t
{
private:
	double sigma;
	double b;
	double r;
	Vector<double> null_state_;
public:
	LorenceAttractor() : r(28.0), b(8.0 / 3), sigma(10.0), null_state_({ 12, 7.3, 5.2 })
	{ }

	void calculateDerivative(double time, const Vector<double>& state, Vector<double>& derivative) const override
	{
		derivative[0] = sigma * (state[1] - state[0]);
		derivative[1] = state[0] * (r - state[2]) - state[1];
		derivative[2] = state[0] * state[1] - b * state[2];
	}

	virtual void setSettings(const std::map<std::string, std::string>& settings) override
	{
		const std::map<std::string, std::string>::const_iterator nope = settings.end();
		std::map<std::string, std::string>::const_iterator pair;

		auto set = [&settings, &pair, &nope](double& var, const char* name) -> void
		{
			if ((pair = settings.find(name)) != nope)
			{
				var = std::atof(pair->second.c_str());
			}
		};

		set(sigma, "sigma");
		set(b, "b");
		set(r, "r");
		set(null_state_[0], "x0");
		set(null_state_[1], "y0");
		set(null_state_[2], "z0");
	}

	virtual void setNullStateSettings(const std::map<std::string, std::string>& settings) override
	{
		const std::map<std::string, std::string>::const_iterator nope = settings.end();
		std::map<std::string, std::string>::const_iterator pair;

		auto set = [&settings, &pair, &nope](double& var, const char* name) -> void
		{
			if ((pair = settings.find(name)) != nope)
			{
				var = std::atof(pair->second.c_str());
			}
		};

		set(null_state_[0], "x0");
		set(null_state_[1], "y0");
		set(null_state_[2], "z0");
	}

	virtual Vector<double> getNullState() const override
	{
		return null_state_;
	}

	virtual size_t getDimension() const override
	{
		return 3;
	}

	virtual ~LorenceAttractor()
	{ }
};

class DoublePendulum : public ode_t
{
private:
	double m;
	double L;
	double g;
	Vector<double> null_state_;
public:
	DoublePendulum() :
		m(1.0), L(1.2), g(9.8), null_state_({ 0, 0, 0, 0 })
	{ }
	// { theta1, theta2, p1, p2 }
	void calculateDerivative(double time, const Vector<double>& state, Vector<double>& derivative) const override
	{
		double k = 6.0 / (m * L * L);
		double arg = state[0] - state[1];
		double denominator = (16.0 - 9.0 * std::cos(arg) * std::cos(arg));

		derivative[0] = k * (2.0 * state[2] - 3.0 * std::cos(arg) * state[3]) / denominator;
		derivative[1] = k * (8.0 * state[3] - 3.0 * std::cos(arg) * state[2]) / denominator;
		derivative[2] = (-0.5 * m * L * L) * (derivative[0] * derivative[1] * std::sin(arg) + 3 * (g / L) * std::sin(state[0]));
		derivative[3] = (-0.5 * m * L * L) * (-derivative[0] * derivative[1] * std::sin(arg) + (g / L) * std::sin(state[1]));
	}

	void setSettings(const std::map<std::string, std::string>& settings) override
	{
		const std::map<std::string, std::string>::const_iterator nope = settings.end();
		std::map<std::string, std::string>::const_iterator pair;

		auto set = [&settings, &pair, &nope](double& var, const char* name) -> void
		{
			if ((pair = settings.find(name)) != nope)
			{
				var = std::atof(pair->second.c_str());
			}
		};

		set(m, "m");
		set(L, "L");
		set(g, "g");
		set(null_state_[0], "theta1");
		set(null_state_[1], "theta2");
		set(null_state_[2], "p1");
		set(null_state_[3], "p2");
	}

	virtual void setNullStateSettings(const std::map<std::string, std::string>& settings) override
	{
		const std::map<std::string, std::string>::const_iterator nope = settings.end();
		std::map<std::string, std::string>::const_iterator pair;

		auto set = [&settings, &pair, &nope](double& var, const char* name) -> void
		{
			if ((pair = settings.find(name)) != nope)
			{
				var = std::atof(pair->second.c_str());
			}
		};

		set(null_state_[0], "theta1");
		set(null_state_[1], "theta2");
		set(null_state_[2], "p1");
		set(null_state_[3], "p2");
	}

	virtual Vector<double> getNullState() const override
	{
		return null_state_;
	}

	virtual size_t getDimension() const override
	{
		return 4;
	}

	virtual ~DoublePendulum()
	{ }
};


class settings_t
{
private:
	std::map<std::string, std::string>& settings;
public:
	void push(const std::string&, const std::string&);
	const std::string& get(const std::string&);
};
