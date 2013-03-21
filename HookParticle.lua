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

Projectile = {
	cast_sound = "sounds/hook.wav";
	impact_sound = "sounds/hook_impact.wav";
	speed = 0.28;
	max_distance = 40.0;
	impact_impulse = -0.4;
}

ParticleSystemData = {
	accel_x = 0;
	accel_y = 0;
	accel_z = 0;
	spawn_frequency = 0.03;
	lifetime = 5;
	max_particles = 20;
	radius = 2;
}

ParticleData = {
	texture = "textures/bolt.dds",
	size = 13,
	lifetime = 0.6,
	mass = 0.1,
	speed = 1,
}

LightData = {
	color = "107 140 255 0";
	intensity = "0 1 1";
	att = "1 0.1 0.05";
	spot = 64;
}

-- Updates the particle with a custom function.
-- Don't have to return anything since particle acts like a pointer.
function UpdateParticle(particle, time)
	-- Calculate the particles age.
	age = time - particle:GetInitialTime();

	-- Calculate position.
	pos_x = particle:GetInitPosX() + particle:GetInitVelX() * age + 0.5 * GetAccelX() * age * age;
	pos_y = 2;--particle:GetInitPosY() + particle:GetInitVelY() * age + 0.5 * GetAccelY() * age * age;
	pos_z = particle:GetInitPosZ() + particle:GetInitVelZ() * age + 0.5 * GetAccelZ() * age * age;

	-- Set new position.
	particle:SetPosition(pos_x, pos_y, pos_z);

	-- Set new size.
	particle:SetSize(particle:GetInitialSize() * math.min((ParticleData.lifetime / age) / 10.0, 1));
end
