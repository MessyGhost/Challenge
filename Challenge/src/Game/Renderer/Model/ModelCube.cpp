#include "ModelCube.h"

ModelCube::ModelCube(
              std::uint32_t px, std::uint32_t nx,
              std::uint32_t py, std::uint32_t ny,
              std::uint32_t pz, std::uint32_t nz) noexcept {
         setCullableFaceModel(
        BlockModel::BlockFace::PositiveX,
        {
            {1.0f, 0.0f, 1.0f, 0.0f, 0.0f, (float)px},
            {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, (float)px},
            {1.0f, 1.0f, 1.0f, 0.0f, 1.0f, (float)px},

            {1.0f, 1.0f, 0.0f, 1.0f, 1.0f, (float)px},
            {1.0f, 1.0f, 1.0f, 0.0f, 1.0f, (float)px},
            {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, (float)px}
        }
        ).setCullableFaceModel(
        BlockModel::BlockFace::NegativeX,
        {
            {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, (float)nx},
            {0.0f, 0.0f, 1.0f, 1.0f, 0.0f, (float)nx},
            {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, (float)nx},

            {0.0f, 1.0f, 1.0f, 1.0f, 1.0f, (float)nx},
            {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, (float)nx},
            {0.0f, 0.0f, 1.0f, 1.0f, 0.0f, (float)nx}
        }
        ).setCullableFaceModel(
        BlockModel::BlockFace::PositiveY,
        {
            {0.0f, 1.0f, 1.0f, 0.0f, 0.0f, (float)py},
            {1.0f, 1.0f, 1.0f, 1.0f, 0.0f, (float)py},
            {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, (float)py},

            {1.0f, 1.0f, 0.0f, 1.0f, 1.0f, (float)py},
            {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, (float)py},
            {1.0f, 1.0f, 1.0f, 1.0f, 0.0f, (float)py}
        }
        ).setCullableFaceModel(
        BlockModel::BlockFace::NegativeY,
        {
            {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, (float)ny},
            {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, (float)ny},
            {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, (float)ny},

            {1.0f, 0.0f, 1.0f, 1.0f, 1.0f, (float)ny},
            {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, (float)ny},
            {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, (float)ny}
        }
        ).setCullableFaceModel(
        BlockModel::BlockFace::PositiveZ,
        {
            {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, (float)pz},
            {1.0f, 0.0f, 1.0f, 1.0f, 0.0f, (float)pz},
            {0.0f, 1.0f, 1.0f, 0.0f, 1.0f, (float)pz},

            {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, (float)pz},
            {0.0f, 1.0f, 1.0f, 0.0f, 1.0f, (float)pz},
            {1.0f, 0.0f, 1.0f, 1.0f, 0.0f, (float)pz}
        }
        ).setCullableFaceModel(
        BlockModel::BlockFace::NegativeZ,
        {
            {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, (float)nz},
            {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, (float)nz},
            {1.0f, 1.0f, 0.0f, 0.0f, 1.0f, (float)nz},

            {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, (float)nz},
            {1.0f, 1.0f, 0.0f, 0.0f, 1.0f, (float)nz},
            {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, (float)nz}
        }
        );
}
