package demo

import "math/rand"

func random(min, max float32) float32 {
	if min > max {
		panic("min > max")
	}

	return rand.Float32()*(max-min) + min
}
