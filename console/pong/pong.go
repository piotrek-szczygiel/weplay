package pong

import (
	"log"
	"math"
	"math/rand"

	rl "github.com/gen2brain/raylib-go/raylib"
	"github.com/piotrek-szczygiel/raspberry-console/console/controller"
)

type player struct {
	position rl.Vector2
	speed    rl.Vector2
	width    int32
	height   int32
}

type ball struct {
	position rl.Vector2
	speed    rl.Vector2
	radius   float32
}

type Pong struct {
	target rl.RenderTexture2D

	width  int32
	height int32

	players [2]player
	ball    ball
}

func New() *Pong {
	var pong Pong

	pong.width = 1920
	pong.height = 1080
	pong.target = rl.LoadRenderTexture(pong.width, pong.height)
	rl.SetTextureFilter(pong.target.Texture, rl.FilterPoint)

	pong.players[0] = player{
		position: rl.Vector2{X: 15, Y: float32(pong.height/2 - 180/2)},
		speed:    rl.Vector2{},
		width:    15,
		height:   180,
	}

	pong.players[1] = player{
		position: rl.Vector2{X: float32(pong.width - 25), Y: float32(pong.height/2 - 180/2)},
		speed:    rl.Vector2{},
		width:    15,
		height:   180,
	}

	pong.ball = ball{
		position: rl.Vector2{X: float32(pong.width) / 2, Y: float32(pong.height) / 2},
		speed:    computeBallSpeed(rl.Vector2{X: 1, Y: 0}),
		radius:   15,
	}

	return &pong
}

func (pong *Pong) Update([]controller.Event) {
	dt := float64(rl.GetFrameTime())
	var w, s, up, down bool

	if rl.IsKeyDown(rl.KeyW) {
		w = true
	}

	if rl.IsKeyDown(rl.KeyS) {
		s = true
	}

	if rl.IsKeyDown(rl.KeyUp) {
		up = true
	}

	if rl.IsKeyDown(rl.KeyDown) {
		down = true
	}

	pong.updatePositions(dt, w, s, up, down)
}

func (pong *Pong) Draw() {
	rl.BeginTextureMode(pong.target)
	rl.ClearBackground(rl.Black)

	for _, player := range pong.players {
		rl.DrawRectangle(int32(player.position.X), int32(player.position.Y), player.width, player.height, rl.White)
	}

	rl.DrawCircle(int32(pong.ball.position.X), int32(pong.ball.position.Y), pong.ball.radius, rl.RayWhite)
	rl.EndTextureMode()
}

func (pong *Pong) GetTarget() rl.RenderTexture2D {
	return pong.target
}

func (pong *Pong) updatePositions(dt float64, w bool, s bool, up bool, down bool) {
	const playerSpeed = 700
	const ballSpeed = 850
	const friction = 0.8

	if w {
		pong.players[0].speed.Y = -1
	} else if s {
		pong.players[0].speed.Y = 1
	} else {
		pong.players[0].speed.Y = 0
	}

	if up {
		pong.players[1].speed.Y = -1
	} else if down {
		pong.players[1].speed.Y = 1
	} else {
		pong.players[1].speed.Y = 0
	}

	for i := range pong.players {
		pong.players[i].position.Y += pong.players[i].speed.Y * playerSpeed * float32(dt)

		if pong.players[i].position.Y < 10 {
			pong.players[i].position.Y = 10
		} else if int32(pong.players[i].position.Y)+pong.players[i].height > pong.height-10 {
			pong.players[i].position.Y = float32(pong.height) - float32(pong.players[i].height) - 10
		}

		playerRect := rl.Rectangle{
			X:      pong.players[i].position.X,
			Y:      pong.players[i].position.Y,
			Width:  float32(pong.players[i].width),
			Height: float32(pong.players[i].height),
		}

		if rl.CheckCollisionCircleRec(pong.ball.position, pong.ball.radius, playerRect) {
			if pong.ball.position.X > 19 && pong.ball.position.X < float32(pong.width)-19 {
				pong.ball.speed = computeBallSpeed(rl.Vector2{
					X: -pong.ball.speed.X,
					Y: pong.ball.speed.Y + pong.players[i].speed.Y*friction,
				})

				if pong.ball.position.X < float32(pong.width)/2 {
					pong.ball.position.X = pong.players[i].position.X + float32(pong.players[i].width) + pong.ball.radius + 1
				} else {
					pong.ball.position.X = pong.players[i].position.X - pong.ball.radius - 1
				}
			}
		}
	}

	if pong.ball.position.Y < pong.ball.radius {
		pong.ball.position.Y = pong.ball.radius
		pong.ball.speed.Y *= -1
	} else if pong.ball.position.Y > float32(pong.height)-pong.ball.radius {
		pong.ball.position.Y = float32(pong.height) - pong.ball.radius
		pong.ball.speed.Y *= -1
	}

	pong.ball.position.X += pong.ball.speed.X * ballSpeed * float32(dt)
	pong.ball.position.Y += pong.ball.speed.Y * ballSpeed * float32(dt)

	if pong.ball.position.X < 15 {
		pong.restart()
	} else if pong.ball.position.X > float32(pong.width)-15 {
		pong.restart()
	}
}

func (pong *Pong) restart() {
	var modAngle = (rand.Float64() * float64(1) / 4 * math.Pi) - float64(1)/8 * math.Pi
	log.Print(modAngle)
	var dirX = pong.ball.speed.X / float32(math.Abs(float64(pong.ball.speed.X))) * -1
	pong.ball = ball{
		position: rl.Vector2{X: float32(pong.width) / 2, Y: float32(pong.height) / 2},
		speed:    computeBallSpeed(rl.Vector2{X: float32(math.Cos(modAngle)) * dirX, Y: float32(math.Sin(modAngle))}),
		radius:   15,
	}
	pong.players[0].position = rl.Vector2{X: 15, Y: float32(pong.height/2 - pong.players[0].height/2)}
	pong.players[1].position = rl.Vector2{X: float32(pong.width - 25), Y: float32(pong.height/2 - pong.players[1].height/2)}
}

func computeBallSpeed(v rl.Vector2) rl.Vector2 {
	var length = float32(math.Sqrt(float64(v.X*v.X + v.Y*v.Y)))
	v = rl.Vector2{X: v.X / length, Y: v.Y / length}
	var vXDir = float64(v.X) / math.Abs(float64(v.X))
	var vYDir = float64(v.Y) / math.Abs(float64(v.Y))
	if math.Abs(float64(v.Y)) >= 0.90 {
		v.Y = float32(0.90 * vYDir)
		v.X = float32(math.Sqrt(float64(1-v.Y*v.Y)) * vXDir)
	}
	return v
}
