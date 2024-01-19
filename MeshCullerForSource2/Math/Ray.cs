using System.Numerics;

namespace MeshCullerForSource2.Math;

internal class Ray(Vector3 Origin, Vector3 direction)
{
	public readonly Vector3 Origin = Origin;
	public readonly Vector3 Direction = direction;

	public class RayHitResult
	{
		public bool Hit;
		public float HitDistance;
		public Vector3? HitPosition;
		public Vector3? HitNormal;
	}
	
	public RayHitResult IntersectsTriangle(Vector3 a, Vector3 b, Vector3 c) {
		var E1 = b - a;
		var E2 = c - a;
		var N = Vector3.Cross(E1, E2);
		float det = -Vector3.Dot(this.Direction, N);
		float invdet = 1.0f / det;
		var AO = this.Origin - a;
		var DAO = Vector3.Cross(AO, this.Direction);
		float u = Vector3.Dot(E2, DAO) * invdet;
		float v = -Vector3.Dot(E1, DAO) * invdet;
		float t = Vector3.Dot(AO, N) * invdet;
		var hit = det >= 1e-6f && t >= 0.0f && u >= 0.0f && v >= 0.0f && (u + v) <= 1.0f;

		Vector3? hitPosition = null;
		Vector3? hitNormal = null;

		if (hit) {
			hitPosition = this.Origin + this.Direction * t;
			hitNormal = Triangle.CalculateSurfaceNormal(a, b, c);
		}

		return new RayHitResult {
			Hit = hit,
			HitDistance = t,
			HitPosition = hitPosition,
			HitNormal = hitNormal
		};
	}
}
