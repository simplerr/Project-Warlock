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
	vel_x = math.cos(-counter/5.0) * ParticleData.speed;		-- X velocity
	vel_y = 0;		-- Y velocity
	vel_z = math.sin(counter/5.0) * ParticleData.speed;		-- Z velocity
	particle:SetInitialVelocity(vel_x, vel_y, vel_z);

	--particle:Debug();

	-- Return the new particle
	return particle;
end

Projectile = {
	cast_sound = "sounds/frostnova.wav";
	impact_sound = "sounds/fireball_impact.wav";
	speed = 0.05;
	max_distance = 40.0;
	impact_impulse = 0.8;
}

ParticleSystemData = {
	accel_x = 0;
	accel_y = 0;
	accel_z = 0;
	spawn_frequency = 0.00002;
	lifetime = 0.4;
	max_particles = 32;
	radius = 5;
}

ParticleData = {
	texture = "textures/smoke.dds",
	size = 5,
	lifetime = 0.4,
	mass = 0.1,
	speed = 28,
}

LightData = {
	color = "69 66 255 0";
	intensity = "0 1 1";
	att = "1 0.1 0.05";
	spot = 64;
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
	particle:SetSize(particle:GetInitialSize());-- * math.sin(age * 12.0));
end
