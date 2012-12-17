dofile("ParticleFunctions.lua");

math.randomseed(os.time())

ParticleSystemData = {
	accel_x = 0;
	accel_y = -0;
	accel_z = 0;
	spawn_frequency = 0.02;
	lifetime = 5;
	max_particles = 100;
	radius = 5;
}

ParticleData = {
	texture = "textures/torch.png",
	size = 10,
	lifetime = 1.2,
	mass = 0.1,
	speed = 1,
}

-- Updates the particle with a custom function.
-- Don't have to return anything since particle acts like a pointer.
function UpdateParticle(particle, time)
	-- Calculate the particles age.
	age = particle:GetInitialTime() - time;

	-- Calculate position.
	pos_x = particle:GetInitPosX() + particle:GetInitVelX() * age + 0.5 * GetAccelX() * age * age;
	pos_y = particle:GetInitPosY() + particle:GetInitVelY() * age + 0.5 * GetAccelY() * age * age;
	pos_z = particle:GetInitPosZ() + particle:GetInitVelZ() * age + 0.5 * GetAccelZ() * age * age;

	-- Set new position.
	particle:SetPosition(pos_x, pos_y, pos_z);

	-- Set new size.
	particle:SetSize(particle:GetInitialSize() * math.sin(age * 12.0));
end