#include "pch.h"
#include "CapsuleShape.h"


CapsuleShape::CapsuleShape(glm::vec3 halfExtents)
{	
	m_halfExtents = halfExtents;
	//m_shape = new btCapsuleShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
	//m_type = CShapeType::CAPSULE;
};
