dofile("ParticleFunctions.lua");

math.randomseed(os.time())

function InitParticle(pos_x, pos_y, pos_z, time, counter)
	-- Create the particle
	local particle = Particle:new();

	-- Set attributes
	particle:SetInitialPos(pos_x, pos_y, pos_z);	-- Position
	particle:SetInitialTime(time);					-- Initial time
	particle:SetInitialSize(ParticleData.size);		-- Initial size
	particle:SetLifetime(ParticleData.lifetime);	-- Life time
	particle:SetMass(ParticleData.mass);			-- Mass
	particle:SetInitialColor(1, 0, 0, 1);			-- Initial color

	-- Velocity
	vel_x = math.random(-1000, 1000) / 1000.0 * ParticleData.speed;		-- X velocity
	vel_y = math.random(-900, 1000) / 1000.0 * ParticleData.speed;		-- Y velocity
	vel_z = math.random(-1000, 1000) / 1000.0 * ParticleData.speed;		-- Z velocity
	particle:SetInitialVelocity(vel_x, vel_y, vel_z);

	--particle:Debug();

	-- Return the new particle
	return particle;
end

ParticleSystemData = {
	accel_x = 0;
	accel_y = 0;
	accel_z = 0;
	spawn_frequency = 0.02;
	lifetime = 10;
	max_particles = 100;
	radius = 2;
}

ParticleData = {
	texture = "textures/torch.png",
	size = 15,
	lifetime = 2.2,
	mass = 0.1,
	speed = 4,
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
	particle:SetSize(particle:GetInitialSize() * math.sin(age * 6.0));
end