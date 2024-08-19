#include "Cheat.h"
#include "Java.h"
#include <thread>
#include <chrono>

jclass getMinecraftClass()
{
	return ct.env->FindClass("ave");
}

jobject getMinecraft() 
{
	jmethodID getMinecraftMethod = ct.env->GetStaticMethodID(getMinecraftClass(), "A", "()Lave;"); 
	return ct.env->CallStaticObjectMethod(getMinecraftClass(), getMinecraftMethod);
}

jobject getWorld()
{
	jfieldID getWorldField = ct.env->GetFieldID(getMinecraftClass(), "f", "Lbdb;");
	return ct.env->GetObjectField(getMinecraft(), getWorldField);
}

jobject getPlayer() 
{
	jfieldID getPlayerField = ct.env->GetFieldID(getMinecraftClass(), "h", "Lbew;");
	return ct.env->GetObjectField(getMinecraft(), getPlayerField);
}

jobject getPlayerConnection()
{
	jfieldID playerConnectionField = ct.env->GetFieldID(ct.env->GetObjectClass(getPlayer()), "a", "Lnh;");
	return ct.env->GetObjectField(getPlayer(), playerConnectionField);
}

void sendPacket(jobject packet)
{
	jclass playerConnectionClass = ct.env->GetObjectClass(getPlayerConnection());
	jmethodID sendPacketMethod = ct.env->GetMethodID(playerConnectionClass, "a", "(Lff;)V");
	ct.env->CallVoidMethod(getPlayerConnection(), sendPacketMethod, packet);
}

void setPlayerSprint()
{
	jmethodID setSprintingMethod = ct.env->GetMethodID(ct.env->GetObjectClass(getPlayer()), "d", "(Z)V");
	ct.env->CallBooleanMethod(getPlayer(), setSprintingMethod, true);
}

void sendPacketToPlayer(jobject packet)
{
	jobject playerConnection = getPlayerConnection();
	jclass playerConnectionClass = ct.env->GetObjectClass(playerConnection);
	jmethodID sendPacketMethod = ct.env->GetMethodID(playerConnectionClass, "a", "(Lff;)V");
	ct.env->CallVoidMethod(playerConnection, sendPacketMethod, packet);
}

void sendParticleEffectToPlayer(jobject player)
{
	jobject particlePacket = createParticlePacket("explode", 0.0f, 100.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.1f, 10);
	sendPacketToPlayer(particlePacket);
}

jobject createParticlePacket(const char* particleName, float x, float y, float z, float offsetX, float offsetY, float offsetZ, float speed, int count)
{
	jclass particlePacketClass = ct.env->FindClass("gp");
	jmethodID particlePacketConstructor = ct.env->GetMethodID(particlePacketClass, "<init>", "(Ljava/lang/String;FFFFFIF)V");

	jstring jParticleName = ct.env->NewStringUTF(particleName);
	return ct.env->NewObject(particlePacketClass, particlePacketConstructor, jParticleName, x, y, z, offsetX, offsetY, offsetZ, speed, count);
}

void sendParticleEffect()
{
	jobject particlePacket = createParticlePacket("explode", 0.0f, 100.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.1f, 10);
	sendPacket(particlePacket);
}

void detectPlayerJoin(JNIEnv* env) 
{
	jobject minecraft = getMinecraftClass();
	jclass clientPlayNetworkHandlerClass = env->FindClass("cls");
	jmethodID onGameJoinMethod = env->GetMethodID(clientPlayNetworkHandlerClass, "onGameJoin", "()V");

	if (onGameJoinMethod != NULL) 
	{
		env->CallVoidMethod(minecraft, onGameJoinMethod);
		printf("Player has joined the game!\n");
	}
	else 
	{
		printf("Failed to find onGameJoin method.\n");
	}
}

void runModules()
{
	while (true)
	{
		if (!getMinecraft()) continue;
		if (!getWorld()) continue;
		if (!getPlayer()) continue;
		
		setPlayerSprint();

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}
