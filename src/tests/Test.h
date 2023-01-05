#pragma once

namespace test {
	class Test {
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(double deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};
}