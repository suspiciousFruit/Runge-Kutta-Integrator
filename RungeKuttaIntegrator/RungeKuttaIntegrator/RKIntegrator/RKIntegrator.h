#pragma once
#include <map>
#include <string>
#include "..\Vector.h"



template <typename Ode, typename RKMethod>
class RKIntegrator : public RKMethod
{
private:
	// Параметры которые относятся к ОДУ
	Ode ode_;
	// Параметры которые относятся методу Рунге-Кутте
	using RKMethod::A;
	using RKMethod::b_main;
	using RKMethod::b_subs;
	using RKMethod::c;
	const size_t rk_steps_number_;
	// Парметры которые относятся к интегратору
	size_t iteration_number_;
	double timespan_;
	double null_time_;
	bool is_main_;
public:
	RKIntegrator() : RKMethod(), rk_steps_number_(c.size()), iteration_number_(1),
		is_main_(true), null_time_(0), timespan_(0.05)
	{ }

	void setSettings(const std::map<std::string, std::string>& settings)
	{
		ode_.setSettings(settings);

		const auto nope = settings.end();
		std::map<std::string, std::string>::const_iterator pair;

		if ((pair = settings.find("iteration_number")) != nope)
			iteration_number_ = std::atoi(pair->second.c_str());

		if ((pair = settings.find("null_time")) != nope)
			null_time_ = std::atof(pair->second.c_str());

		if ((pair = settings.find("timespan")) != nope)
			timespan_ = std::atof(pair->second.c_str());

		if ((pair = settings.find("is_main")) != nope)
		{
			if (pair->second.c_str() == "true")
				is_main_ = true;
			else if (pair->second.c_str() == "false")
				is_main_ = false;
		}
	}
	// Функция которая интегрирует ОДУ в переданный файл
	void integrateInFile(std::ofstream& file_out)
	{
		// Получаем от ОДУ начальное положение системы
		Vector<double> null_state = ode_.getNullState();
		// Инициализируем вектора
		Vector<double> a(ode_.getDimention());
		Vector<Vector<double>> k(rk_steps_number_);
		Vector<double> res(ode_.getDimention());
		// Выбираем вектор по которому будет проходить суммирование
		const Vector<double>& b = (is_main_ ? b_main : b_subs);
		// В цикле интегрируем
		for (size_t n = 0; n < iteration_number_; ++n)
		{
			res.apply([](double) -> double { return 0.0; });

			for (size_t i = 0; i < rk_steps_number_; ++i)
			{
				a.apply([](double a) -> double { return 0.0; });

				for (size_t j = 0; j < i; ++j)
					a += A[i][j] * k[j];
				(a *= timespan_) += null_state;

				k[i] = ode_(null_time_ + timespan_ * c[i], a);

				res += k[i] * b[i];
			}
			res *= timespan_;
			res += null_state;
			// Выводим в файл
			file_out << res << std::endl;
			// Переходим к следующему начальному положению
			null_state = res;
			null_time_ += timespan_;
		}
	}
};

#include "ode_t.h"
#include "rkmethod_t.h"

// Считается что интегратор не может менять метод Рунге-Кутта и тип ОДУ внутри себя
// Однако позже имеет смысл добавить подобную функциональность (упаковать метод и ОДУ в обертки, которые могут изменять типы)
class _RKIntegrator
{
private:
	ode_t* ode_;
	const rkmethod_t* rkmethod_;
	size_t iteration_number_;
	double timespan_;
	double null_time_;
	bool is_main_;
public:
	_RKIntegrator(ode_t* ode, const rkmethod_t* rkmethod) :
		ode_(ode), rkmethod_(rkmethod), iteration_number_(0),
		is_main_(true), timespan_(0.05), // Значение для шага по умолчанию
		null_time_(0.0)
	{ }

	inline void setOdeSettings(const std::map<std::string, std::string>& settings) // Установка параметров модели
	{
		if (ode_) ode_->setSettings(settings);
	}
	inline void setNullStateSettings(const std::map<std::string, std::string>& settings) // Установка начальных состояний (nuLL_time)
	{
		if (ode_) ode_->setNullStateSettings(settings);
	}

	// Настройка ветора b выполняется в интеграторе
	//void setRKMethodSettings(const std::map<std::string, std::string>&); // Установка настроек метода Рунге-Кутте (isMain)

	void setIntegratorSettings(const std::map<std::string, std::string>& settings) // Установка настроек интегратора (шаг, количество шагов)
	{
		std::map<std::string, std::string>::const_iterator pair;
		const std::map<std::string, std::string>::const_iterator nope = settings.end();

		/*
			timespan: size_t
			null_time: double
			iteration_number: size_t
			is_main: {true, false}
		*/

		if ((pair = settings.find("timespan")) != nope)
		{
			timespan_ = std::atoi(pair->second.c_str());
		}

		if ((pair = settings.find("iteration_number")) != nope)
		{
			iteration_number_ = std::atoi(pair->second.c_str());
		}

		if ((pair = settings.find("null_time")) != nope)
		{
			null_time_ = std::atof(pair->second.c_str());
		}

		if ((pair = settings.find("is_main")) != nope)
		{
			if (pair->second == "true")
				is_main_ = true;
			else
				is_main_ = false;
		}
	}

	void setFullSettings(const std::map<std::string, std::string>& settings) // Полная установка настроек
	{
		if (ode_)
		{
			ode_->setSettings(settings);
			ode_->setNullStateSettings(settings);
		}
			
		this->setIntegratorSettings(settings);
	}

	void integrateInFile(std::ostream& file);
};

void _RKIntegrator::integrateInFile(std::ostream& file)
{
	// Получаем от ОДУ начальное положение системы
	Vector<double> null_state = ode_->getNullState();	
	// Количество шагов в методе Рунге-Кутты
	const size_t rk_steps_number = rkmethod_->c.size();
	// Размерность вектора состояние ОДУ
	const size_t dimesion = ode_->getDimension();
	// Вектор для промежуточного состояния
	Vector<double> a(dimesion);
	// Вектор для записи в него результата
	Vector<double> res(dimesion);
	// Инициализируем массив векторов для записи в них промежуточных значений состояний
	Vector<Vector<double>> k(rk_steps_number);
	for (size_t i = 0; i < k.size(); ++i)
		k[i].resize(dimesion);
	// Выбираем вектор по которому будет проходить суммирование
	const Vector<double>& const b = (is_main_ ? rkmethod_->b_main : rkmethod_->b_subs);
	// 
	const Matrix<double>& const A = rkmethod_->A;
	const Vector<double>& const c = rkmethod_->c;

	// Копируем начальное время для того чтобы не модифицировать состояние интегратора
	double null_time = null_time_;

	for (size_t n = 0; n < iteration_number_; ++n)
	{
		res.apply([](double) -> double { return 0.0; });

		for (size_t i = 0; i < rk_steps_number; ++i)
		{
			a.apply([](double a) -> double { return 0.0; });

			for (size_t j = 0; j < i; ++j)
				a += A[i][j] * k[j];

			a *= timespan_;
			a += null_state;
			// Считается что последний аргумент той же размерности что и вектор состояния ОДУ
			ode_->calculateDerivative(null_time + timespan_ * c[i], a, k[i]);
			//////////////////////////////////////////////////////////////////////////////// указатель на функцию

			res += k[i] * b[i];
		}
		res *= timespan_;
		res += null_state;
		// Выводим в файл
		file << res << std::endl;
		// Переходим к следующему начальному положению
		null_state = res;
		null_time += timespan_;
	}
}
