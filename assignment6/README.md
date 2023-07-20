# Assignment 6: Grid Acceleration & Solid Textures

## Optimization analysis

Ryzen 3700x, DDR4 3200MHz 48GB

| Scene                    | Time(naive) | Intersection(naive) | Time(grid) | Intersection(grid) | Acceleration |
| ------------------------ | ----------- | ------------------- | ---------- | ------------------ | ------------ |
| scene6_01_sphere         | 0:00:01     | 40000               | 0:00:01    | 11708(29%)         | 1.0x         |
| sphere_triangles         | 0:00:01     | 120000              | 0:00:01    | 65852(55%)         | 1.0x         |
| sphere_triangles(shadow) | 0:00:01     | 168750              | 0:00:01    | 125980(74%)        | 1.0x         |
| sphere_plane             | 0:00:01     | 80000               | 0:00:01    | 51708(64%)         | 1.0x         |
| sphere_plane(shadow)     | 0:00:01     | 150000              | 0:00:01    | 97360(65%)         | 1.0x         |
| bunny_mesh_200           | 0:00:01     | 8040000             | 0:00:01    | 396270(4.9%)       | 1.0x         |
| bunny_mesh_200(shadow)   | 0:00:01     | 21421776            | 0:00:01    | 1206558(5.6%)      | 1.0x         |
| bunny_mesh_1k(shadow)    | ---         | ---                 | 0:00:01    | 1653102(-%)        | -x           |
| bunny_mesh_5k(shadow)    | ---         | ---                 | 0:00:01    | 2875570(-%)        | -x           |
| bunny_mesh_40k(shadow)   | ---         | ---                 | 0:00:01    | 4298860(-%)        | -x           |
