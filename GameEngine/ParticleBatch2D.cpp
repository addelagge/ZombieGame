#include "ParticleBatch2D.h"


namespace GameEngine{


	ParticleBatch2D::ParticleBatch2D()
	{
	}


	ParticleBatch2D::~ParticleBatch2D()
	{
		delete[] m_particles;		
	}

	void ParticleBatch2D::init(int maxParticles, float decayRate, GLTexture texture, std::function<void(Particle2D&, float)> updateFunc /* = defaultParticleUpdate */)
	{
		m_texture = texture;
		m_decayRate = decayRate;
		m_maxParticles = maxParticles;
		m_particles = new Particle2D[m_maxParticles];
		m_updateFunc = updateFunc;

	}

	void ParticleBatch2D::update(float deltaTime)
	{
		for (int i = 0; i < m_maxParticles; i++){
			//check if active
			if (m_particles[i].life > 0)
			{
				m_updateFunc(m_particles[i], deltaTime);
				m_particles[i].life -= m_decayRate * deltaTime;

			}
		}
	}


	void ParticleBatch2D::draw(SpriteBatch* spriteBatch)
	{
		glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		
		for (int i = 0; i < m_maxParticles; i++){
			//check if active
			if (m_particles[i].life > 0)
			{
				glm::vec4 destRect = glm::vec4(m_particles[i].position.x, m_particles[i].position.y, m_particles[i].width, m_particles[i].width);
				spriteBatch->draw(destRect, uvRect, m_texture.id, 0.0f, m_particles[i].color);
			}
		}

	}

	void ParticleBatch2D::addParticle(const glm::vec2& position, const Color& color, const glm::vec2& velocity, float width)
	{
		m_lastFreeParticle = findFreeParticle();
		m_particles[m_lastFreeParticle].position = position;
		m_particles[m_lastFreeParticle].color = color;
		m_particles[m_lastFreeParticle].velocity = velocity;
		m_particles[m_lastFreeParticle].width = width;
		m_particles[m_lastFreeParticle].life = 1.0f;
		m_lastFreeParticle++;


	}

	int ParticleBatch2D::findFreeParticle()
	{
		for (int i = m_lastFreeParticle; i < m_maxParticles; i++){
			if (m_particles[i].life <= 0.0f) {
				m_lastFreeParticle = i;
				return i;
			}
		}

		for (int i = 0; i < m_lastFreeParticle; i++){
			if (m_particles[i].life <= 0.0f) {
				m_lastFreeParticle = i;
				return i;
			}
		}
		//no free particles
		return 0;

	}

}