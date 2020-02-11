#ifndef CONFIGURATOR_INTERFACE_H
#define CONFIGURATOR_INTERFACE_H

namespace I
{
	class Configurator
	{
	public:
		virtual void Configure() = 0;
		virtual void Invalidate() = 0;
	};
}

#endif