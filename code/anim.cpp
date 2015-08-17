namespace anim
{
	struct Tween
	{
		f32* value;
		f32 t;
		f32 step;
		f32 from;
		f32 to;
		i32 loop;
		i32 loops;
		b32 playing;
		Bezier curve;
	};	

	internal Tween
	from_to(f32* value, f32 from, f32 to, f32 duration, i32 loops, Bezier curve)
	{
		Tween r;
		r.value = value;
		r.t = 0.0f;
		r.step = 1.0f / duration;
		r.from = from;
		r.to = to;
		r.loops = loops;
		r.loop = loops;
		r.curve = curve;
		r.playing = false;
		return r;
	}

	internal void
	update(Tween* t, f32 dt)
	{
		t->t += t->step * dt;
		if(t->t > 1.0f)
		{
			t->t = 1.0f;
			t->loop--;
			if(t->loop == 0)
			{
				t->t = 1.0f;
				t->playing = false;
			}
			else
			{
				t->t = 0.0f;
			}
		}
		*t->value = t->from + bezier::evaluate(t->curve, t->t) * (t->to - t->from);
	}

	internal void
	play(Tween* t)
	{
		t->loop = t->loops;
		t->t = 0.0f;
		t->playing = true;
	}

}