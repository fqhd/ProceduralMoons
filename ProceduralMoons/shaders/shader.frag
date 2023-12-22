#version 330 core

out vec4 fragColor;

void main()
{
    // Define the screen resolution
    vec2 resolution = vec2(2048.0);

    // Define the center and zoom level
    vec2 center = vec2(-0.5, 0.0);
    float zoom = 3.0;

    // Convert pixel coordinates to complex coordinates
    vec2 c = (gl_FragCoord.xy - 0.5 * resolution) / resolution * zoom + center;

    // Mandelbrot set parameters
    const int maxIterations = 100000;
    const float escapeRadius = 16.0;

    // Initialize variables
    vec2 z = vec2(0.0, 0.0);
    int iterations = 0;

    // Iterate to determine if the point is in the Mandelbrot set
    while (iterations < maxIterations && dot(z, z) < escapeRadius)
    {
        // Mandelbrot iteration formula
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;

        iterations++;
    }

    // Map iteration count to color gradient
    vec3 color = vec3(0.0);
    if (iterations < maxIterations)
    {
        float t = float(iterations) / float(maxIterations);
        color = vec3(sin(6.28318 * t), cos(6.28318 * t), sin(6.28318 * t * 2.0));
    }

    // Output the final color
    fragColor = vec4(color, 1.0);
}