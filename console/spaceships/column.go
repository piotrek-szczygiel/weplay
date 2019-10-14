package spaceships

import (
	"github.com/chewxy/math32"
	rl "github.com/gen2brain/raylib-go/raylib"
	u "github.com/piotrek-szczygiel/raspberry-console/console/utils"
)

type column struct {
	position rl.Vector3
	width    float32
	height   float32
	length   float32
	color    rl.Color
}

func newColumn(x, y, z float32, width, height, length float32, color rl.Color) column {
	return column{
		position: rl.Vector3{X: x, Y: y, Z: z},
		width:    width,
		height:   height,
		length:   length,
		color:    color,
	}
}

func (column *column) draw() {
	rl.DrawCube(column.position, column.width, column.height, column.length, column.color)
}

func generateColumns(n int, mapLength, mapWidth, maxHeight float32) []column {
	var columns []column

	for i := 0; i < n; i++ {
		height := u.Random(6, maxHeight)

		if i%10 == 0 {
			columns = append(columns, newColumn(
				math32.Trunc(u.Random(50, mapLength)),
				math32.Trunc(u.Random(1, height)),
				0,
				2,
				2,
				mapWidth,
				randomColor()))
		} else {
			columns = append(columns, newColumn(
				math32.Trunc(u.Random(50, mapLength)),
				height/2,
				math32.Trunc(u.Random(-mapWidth/2, mapWidth/2)),
				2,
				height,
				2,
				randomColor()))
		}
	}
	return columns
}

func randomColor() rl.Color {
	return rl.Color{
		R: uint8(u.Random(20, 255)),
		G: uint8(u.Random(10, 55)),
		B: uint8(u.Random(30, 60)),
		A: uint8(u.Random(200, 255)),
	}
}
