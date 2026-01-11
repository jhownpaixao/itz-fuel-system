modded class ActionFillFuel
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !target || !IsTransport(target) )
			return false;

		if ( item.GetQuantity() <= 0 )
			return false;

		if (target.GetObject().IsDamageDestroyed())
			return false;
		
		CarScript car = CarScript.Cast(target.GetObject());
		BoatScript boat = BoatScript.Cast(target.GetObject());
		if (car)
		{
			if (car.GetFluidFraction( CarFluid.FUEL ) >= 0.98 || car.GetFuelType() != item.GetLiquidType())
				return false;
		}
		else if (boat)
		{
			if ( boat.GetFluidFraction( BoatFluid.FUEL ) >= 0.98 || boat.GetFuelType() != item.GetLiquidType())
				return false;
		}
		else 
			return false;

		array<string> selections = new array<string>;
		target.GetObject().GetActionComponentNameList(target.GetComponentIndex(), selections);

		Transport vehicle = Transport.Cast(target.GetObject());
		
		if ( vehicle )
		{
			for (int s = 0; s < selections.Count(); s++)
			{
				if ( selections[s] == vehicle.GetActionCompNameFuel() )
				{
					float dist = vector.DistanceSq( vehicle.GetRefillPointPosWS(), player.GetPosition() );

					if ( dist < vehicle.GetActionDistanceFuel() * vehicle.GetActionDistanceFuel() )
						return true;
				}
			}
		}

		return false;
	}
};