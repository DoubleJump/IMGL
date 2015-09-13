namespace physics
{
	struct Rigidbody
	{
		u32 id;
		Vec3 position;
		f32 mass;
		f32 inv_mass;
		Vec3 acceleration;
		Vec3 velocity;
		f32 restitution;
		f32 friction;
	};

	struct Manifold
	{
		Rigidbody* A; //TODO: change these to ids
		Rigidbody* B;
		Vec3 normal;
		f32 penetration;
	};

	fn void
	integrate_euler(Rigidbody* b, f32 dt)
	{
		auto dts = dt * dt;
		b->velocity += b->acceleration * dts * 0.5f;
		b->position += b->velocity * dt;
		b->acceleration = { 0,0,0 };
	}

	fn void
	add_force(Rigidbody* b, Vec3 force)
	{
		b->acceleration += force * b->inv_mass;
	}

	fn void
	apply_impulse(Rigidbody* body, Vec3 impulse)
	{
		body->velocity += impulse * body->inv_mass;
	}

	fn void
	resolve_manifold(Manifold m)
	{
		Vec3 relative_velocity =  m.B->velocity - m.A->velocity;
		f32 velocity_along_normal = vec3::dot(relative_velocity, m.normal);

		if(velocity_along_normal > 0)
		{
			return;
		}

		f32 restitution = math::min(m.A->restitution, m.B->restitution);

		f32 impulse_magnitude = -(1 + restitution) * velocity_along_normal;
		impulse_magnitude /= m.A->inv_mass + m.B->inv_mass;

		Vec3 impulse = m.normal * impulse_magnitude;
		apply_impulse(m.A, -impulse);
		apply_impulse(m.B, impulse);

		m.A->velocity -= m.A->inv_mass * impulse;
		m.B->velocity += m.B->inv_mass * impulse;
	}


}