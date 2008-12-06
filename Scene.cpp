#include <iostream>
#include <vector>	
#include <Scene.h>
#include <libmath/Vector3.h>
#include <drawables/Sphere.h>
#include <shaders/DefaultShader.h>
#include <lights/SimpleLight.h>

using Math::Vector3d;

Scene::Scene()
{
#if 1
	ShaderPtr shader = ShaderPtr(new DefaultShader);

	for (int i=0; i < 100; i++) {
		Sphere *s = new Sphere(Vector3d((rand()%1000 - 500) / 10.0f, (rand()%1000 - 500) / 10.0f, (rand()%1000 - 500) / 10.0f), (rand()%200) / 10.0f);
		s->SetShader(shader);
		m_DrawableList.push_back(s);
	}
#endif
	
	m_DrawableList.push_back(new Sphere(Vector3d(0.0, 0.0, 0.0), 0.25));
	m_DrawableList.push_back(new Sphere(Vector3d(0.5, 0.0, 0.0), 0.25));
	m_DrawableList.push_back(new Sphere(Vector3d(0.5, 0.5, 0.0), 0.25));
	m_DrawableList.push_back(new Sphere(Vector3d(0.5, 0.6, 0.5), 0.10));

	m_SimpleLightList.push_back(new SimpleLight(Vector3d(0.0, 0.0, 100.0), colorf(1.0, 0.0, 0.0), 100));
	m_SimpleLightList.push_back(new SimpleLight(Vector3d(100.0, 0.0, 0.0), colorf(0.0, 1.0, 0.0), 100));
	m_SimpleLightList.push_back(new SimpleLight(Vector3d(0.0, 100.0, 0.0), colorf(0.0, 0.0, 1.0), 100));
}

Scene::~Scene()
{
	// empty our list
	while(!m_DrawableList.empty()) {
		Drawable *d = m_DrawableList.back();
		m_DrawableList.pop_back();
		delete d;
	}
}

const Drawable *Scene::Intersect(const Ray &ray)
{
	bool hit = false;
	Vector3d closestPos;
	Drawable *closest = NULL;

	for (std::vector<Drawable *>::const_iterator i = m_DrawableList.begin(); i != m_DrawableList.end(); i++) {
		Drawable *d = *i;

		Vector3d pos;
		Vector3d normal;
		if (d->Intersect(ray, pos, normal)) {
//				std::cout << "collision ray " << ray << " pos " << pos << std::endl;
			if (!hit || (pos - ray.origin).LengthSquared() < (closestPos - ray.origin).LengthSquared()) {
				closestPos = pos;
				closest = d;
			}
			hit = true;
		}
	}

	if (hit)
		return closest;

	return NULL;
}

bool Scene::DoesIntersect(const Ray &ray)
{
	for (std::vector<Drawable *>::const_iterator i = m_DrawableList.begin(); i != m_DrawableList.end(); i++) {
		Drawable *d = *i;

		if (d->Intersect(ray)) {
			return true;
		}
	}

	return false;
}

