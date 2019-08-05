-- Based entirely upon https://nerdyteachers.com/Explain/FruitDrop/ --
local Score = 0
local Level = 1
local Gravity = 80
local Balls = {}
local BallSpr = nil
local BatSpr = nil
local CPixel = nil
local SPixel = nil
local BatHeight = 20
local BallInterval = 50

local function GenerateBalls()
    for i = 1, Level do
        local ball = {
            sprite = BallSpr,
            x = math.random(BallSpr.x, Screen.x - 2),
            y = i * (-BallInterval)
        }
        table.insert(Balls, ball)
    end
end

local function DrawBat(x, offset)
    local rMouseMax = x + BatSpr.x / 2
    x = x - BatSpr.x / 2
    local lMax = 0
    local rMax = Screen.x
    if x < lMax then
        Draw.Sprite(lMax, Screen.y - offset, BatSpr)
    elseif rMouseMax > rMax then
        Draw.Sprite(rMax - BatSpr.x, Screen.y - offset, BatSpr)
    else
        Draw.Sprite(x, Screen.y - offset, BatSpr)
    end
end

function OnUserCreate()
    BallSpr = Sprite.new("Sprites/Ball.jpg")
    BatSpr = Sprite.new("Sprites/Bat.jpg")
    CPixel = Pixel.new(0, 0, 0, 255)
    SPixel = Pixel.new(200, 200, 0, 255)
    GenerateBalls()
end

function OnUserUpdate(fElapsedTime)
    Screen.Clear(CPixel)
    Draw.SetPixelMode(2)

    for k, ball in pairs(Balls) do
        ball.y = ball.y + fElapsedTime * Gravity
        if ball.x + 1 >= IO.mouseX - (BatSpr.x / 2) and ball.x + 1 <= IO.mouseX + (BatSpr.x / 2) then
            if ball.y + BallSpr.y >= Screen.y - BatHeight then
                Score = Score + 1
                table.remove(Balls, k)
            end
        end

        if ball.y > Screen.y then
            table.remove(Balls, k)
        end
    end

    if #Balls == 0 then
        Level = Level + 1
        GenerateBalls()
    end

    DrawBat(IO.mouseX, BatHeight)

    for _, ball in pairs(Balls) do
        Draw.Sprite(ball.x, ball.y, ball.sprite)
    end

    Draw.String(0, 0, string.format("Score = %s", Score), SPixel, 1)
end