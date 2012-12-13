dofile("ParticleFunctions.lua");

math.randomseed(os.time())

ParticleSystemData = {
	accel_x = 0;
	accel_y = -100;
	accel_z = 0;
	spawn_frequency = 0.01;
	lifetime = 5;
	max_particles = 100;
}

ParticleData = {
	texture = "textures/torch.dds",
	size = 15,
	lifetime = 5.0,
	mass = 0.1,
	speed = 50,
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
	particle:SetSize(particle:GetInitialSize() * math.sin(age * 6.0) * 3);
end