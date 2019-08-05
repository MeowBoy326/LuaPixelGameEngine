local pPixel = nil

function OnUserCreate()
    pPixel = Pixel.new(0, 0, 0, 255)
end

function OnUserUpdate(fElapsedTime)
    for x = 0, Screen.x do
        for y = 0, Screen.y do
            pPixel.r = math.random(0, 255)
            pPixel.g = math.random(0, 255)
            pPixel.b = math.random(0, 255)
            Draw.Pixel(x, y, pPixel)
        end
    end
    return true
end