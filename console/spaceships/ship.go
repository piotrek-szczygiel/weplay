package spaceships

import (
	"github.com/chewxy/math32"
	rl "github.com/gen2brain/raylib-go/raylib"
)

type ship struct {
	position rl.Vector3
	speed    rl.Vector3
	camera   camera

	forward bool
	back    bool
	left    bool
	right   bool
	up      bool
	down    bool

	strength float32
}

func newShip(x, y, z, strength float32) ship {
	return ship{position: vec3(x, y, z),
		speed:    vec3(0, 0, 0),
		camera:   newCamera(),
		strength: strength,
	}
}

func (s *ship) update(dt float32) {
	if s.forward && !s.back {
		s.speed.X += dt * s.strength * 4
	} else if s.back && !s.forward {
		s.speed.X -= dt * s.strength
	} else {
		s.speed.X = fade(s.speed.X, 0, dt*s.strength/4)
	}

	if s.left && !s.right {
		if s.speed.Z > 0 {
			s.speed.Z -= dt * s.strength * 4
		} else {
			s.speed.Z -= dt * s.strength * 8
		}
	} else if s.right && !s.left {
		if s.speed.Z < 0 {
			s.speed.Z += dt * s.strength * 4
		} else {
			s.speed.Z += dt * s.strength * 8
		}
	} else {
		s.speed.Z = fade(s.speed.Z, 0, dt*s.strength*2)
	}

	if s.up && !s.down {
		s.speed.Y += dt * s.strength * 2
	} else if s.down && !s.up {
		s.speed.Y -= dt * s.strength * 2
	} else {
		s.speed.Y = fade(s.speed.Y, 0, dt*s.strength*4)
	}

	s.speed.X = clamp(s.speed.X, -s.strength/4, s.strength*2)
	s.speed.Y = clamp(s.speed.Y, -s.strength, s.strength)
	s.speed.Z = clamp(s.speed.Z, -s.strength/2, s.strength/2)

	s.camera.angle.Z = s.speed.Z / s.strength * math32.Pi / 8

	s.position = addVec3(s.position, s.speed)
	s.camera.c.Position = s.position
	s.camera.update()
}

func (s *ship) getCamera() rl.Camera {
	return s.camera.c
}
