#pragma once

#include <glm/glm.hpp>


namespace Hildur {


	class Primitives2D {

	public:

		struct Tri {
		public:
			Tri(glm::vec2 pos = { 0.0f, 0.0f }, glm::vec2 scale = { 0.0f, 0.0f })
				: Pos(pos), Scale(scale) {}


		private:

			glm::vec2 Pos;
			glm::vec2 Scale;

		};

	};


}