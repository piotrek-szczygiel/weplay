package spaceships

import (
	"math/rand"

	rl "github.com/gen2brain/raylib-go/raylib"
)

func random(min, max float32) float32 {
	if min > max {
		panic("min > max")
	}

	return rand.Float32()*(max-min) + min
}

func vec3(x, y, z float32) rl.Vector3 {
	return rl.Vector3{X: x, Y: y, Z: z}
}

func vec2(x, y float32) rl.Vector2 {
	return rl.Vector2{X: x, Y: y}
}

func addVec3(v1, v2 rl.Vector3) rl.Vector3 {
	return vec3(v1.X+v2.X, v1.Y+v2.Y, v1.Z+v2.Z)
}

func clamp(x, min, max float32) float32 {
	if x < min {
		return min
	} else if x > max {
		return max
	} else {
		return x
	}
}

func fade(x, dest, offset float32) float32 {
	if x > dest {
		x -= offset
		if x < dest {
			x = dest
		}
	} else if x < -dest {
		x += offset
		if x > dest {
			x = dest
		}
	}

	return x
}
