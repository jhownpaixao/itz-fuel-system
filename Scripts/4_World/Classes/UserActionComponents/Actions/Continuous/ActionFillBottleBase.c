modded class ActionFillBottleBase
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		FuelStation station;
		if (Class.CastTo(station, target.GetParent()) || Class.CastTo(station, target.GetObject()))
		{
			return false;
		}

		return super.ActionCondition(player, target, item);
	}
}
