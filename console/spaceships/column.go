package spaceships

import rl "github.com/gen2brain/raylib-go/raylib"

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
	rl.DrawCube(column.position, 2, column.height, 2, column.color)
}

func generateColumns(n int, mapLength, mapWidth float32) []column {
	columns := make([]column, n)
	for i := 0; i < n; i++ {
		height := random(6, 30)
		columns[i] = newColumn(
			random(50, mapLength),
			height/2,
			random(-mapWidth/2, mapWidth/2),
			2,
			height,
			2,
			rl.Color{
				R: uint8(random(20, 255)),
				G: uint8(random(10, 55)),
				B: uint8(random(30, 60)),
				A: uint8(random(200, 255)),
			})
	}
	return columns
}
