package pong

import (
	rl "github.com/gen2brain/raylib-go/raylib"
	"github.com/piotrek-szczygiel/raspberry-console/console/controller"
	"log"
	"math"
)

const PlayerSpeed = 425
const BallSPeed = 450

type Player struct {
	position rl.Vector2
	speed    rl.Vector2
	width    int32
	height   int32
}

type Ball struct {
	position rl.Vector2
	speed    rl.Vector2
	radius   float32
}

type Pong struct {
	target rl.RenderTexture2D

	players [2]Player
	ball    Ball
}

func New() *Pong {
	var pong Pong

	pong.target = rl.LoadRenderTexture(1920, 1080)
	rl.SetTextureFilter(pong.target.Texture, rl.FilterAnisotropic16x)

	pong.players[0] = Player{
		rl.Vector2{X: 15, Y: 10},
		rl.Vector2{},
		15,
		150,
	}

	pong.players[1] = Player{
		rl.Vector2{X: float32(rl.GetScreenWidth() - 25), Y: 10},
		rl.Vector2{},
		15,
		150,
	}

	pong.ball = Ball{
		rl.Vector2{X: float32(rl.GetScreenWidth() / 2), Y: float32(rl.GetScreenHeight() / 2)},
		rl.Vector2{X: BallSPeed, Y: BallSPeed},
		15,
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
	rl.ClearBackground(rl.Black)

	for _, player := range pong.players {
		rl.DrawRectangle(int32(player.position.X), int32(player.position.Y), player.width, player.height, rl.White)
	}

	rl.DrawCircle(int32(pong.ball.position.X), int32(pong.ball.position.Y), pong.ball.radius, rl.RayWhite)
}

func (pong *Pong) GetTarget() rl.RenderTexture2D {
	return pong.target
}

func (pong *Pong) updatePositions(dt float64, w bool, s bool, up bool, down bool) {
	if w {
		pong.players[0].speed.Y = -PlayerSpeed
	} else if s {
		pong.players[0].speed.Y = PlayerSpeed
	} else {
		pong.players[0].speed.Y = 0
	}

	if up {
		pong.players[1].speed.Y = -PlayerSpeed
	} else if down {
		pong.players[1].speed.Y = PlayerSpeed
	} else {
		pong.players[1].speed.Y = 0
	}

	for i := range pong.players {
		pong.players[i].position.Y += pong.players[i].speed.Y * float32(dt)

		if pong.players[i].position.Y < 10 {
			pong.players[i].position.Y = 10
		} else if int32(pong.players[i].position.Y)+pong.players[i].height > int32(rl.GetScreenHeight())-10 {
			pong.players[i].position.Y = float32(rl.GetScreenHeight()) - float32(pong.players[i].height) - 10
		}

		if rl.CheckCollisionCircleRec(
			pong.ball.position,
			pong.ball.radius,
			rl.Rectangle{X: pong.players[i].position.X, Y: pong.players[i].position.Y,
				Width: float32(pong.players[i].width), Height: float32(pong.players[i].height)}) {
			pong.ball.speed = normalize(rl.Vector2{X: -pong.ball.speed.X, Y: pong.ball.speed.Y + pong.players[i].speed.Y})
			pong.ball.speed = rl.Vector2{X: pong.ball.speed.X * BallSPeed, Y: pong.ball.speed.Y * BallSPeed}
		}
	}

	pong.ball.position.X += pong.ball.speed.X * float32(dt)
	pong.ball.position.Y += pong.ball.speed.Y * float32(dt)

	if pong.ball.position.Y < pong.ball.radius {
		pong.ball.position.Y = pong.ball.radius
		pong.ball.speed.Y *= -1
	} else if pong.ball.position.Y > float32(rl.GetScreenHeight())-pong.ball.radius {
		pong.ball.position.Y = float32(rl.GetScreenHeight()) - pong.ball.radius
		pong.ball.speed.Y *= -1
	}
}

func normalize(v rl.Vector2) rl.Vector2 {
	var length = float32(math.Sqrt(float64(v.X*v.X + v.Y*v.Y)))

	log.Print(v)

	return rl.Vector2{X: v.X / length, Y: v.Y / length}
}
