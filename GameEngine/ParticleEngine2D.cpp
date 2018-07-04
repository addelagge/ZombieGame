#include "ParticleEngine2D.h"
#include "ParticleBatch2D.h"
#include "SpriteBatch.h"

namespace GameEngine{

	ParticleEngine2D::ParticleEngine2D()
	{
	}


	ParticleEngine2D::~ParticleEngine2D()
	{
		for (int i = 0; i < m_batches.size(); i++){
			delete m_batches[i];
		}
	}

	void ParticleEngine2D::addParticleBatch(ParticleBatch2D* particleBatch)
	{
		m_batches.push_back(particleBatch);

	}

	void ParticleEngine2D::update(float deltaTime)
	{
		for (int i = 0; i < m_batches.size(); i++){
			m_batches[i]->update(deltaTime);
		}

	}

	void ParticleEngine2D::draw(SpriteBatch* spriteBatch)
	{
		
		for (int i = 0; i < m_batches.size(); i++){
			spriteBatch->begin();
			m_batches[i]->draw(spriteBatch);
			spriteBatch->end();
			spriteBatch->renderBatch();
		}

	}


}