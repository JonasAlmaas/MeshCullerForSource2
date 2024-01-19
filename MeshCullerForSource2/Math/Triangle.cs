using System.Numerics;

namespace MeshCullerForSource2.Math;

internal struct Triangle
{
	// https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
	public static Vector3 CalculateSurfaceNormal(Vector3 a, Vector3 b, Vector3 c) {
		var U = b - a;
		var V = c - a;
		
		return new Vector3 {
			X = U.Y * V.Y - U.Z * V.Y,
			Y = U.Z * V.X - U.X * V.Z,
			Z = U.X * V.Y - U.Y * V.X
		};
	}
}
