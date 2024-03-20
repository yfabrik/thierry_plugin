#include "plugin.hpp"

struct C4_quinte : Module
{
	enum ParamId
	{
		PARAMS_LEN
	};
	enum InputId
	{
		INPUTS_LEN
	};
	enum OutputId
	{
		C4_OUTPUT_OUTPUT,
		SEMITONE_OUTPUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId
	{
		LIGHTS_LEN
	};

	C4_quinte()
	{
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configOutput(C4_OUTPUT_OUTPUT, "");
		configOutput(SEMITONE_OUTPUT_OUTPUT, "");
	}

	float phase = 0.f;
	float phase2 = 0.f;
	void process(const ProcessArgs &args) override
	{

		phase += dsp::FREQ_C4 * args.sampleTime;
		if (phase >= 1.f)
			phase -= 1.f;

		float sine = std::sin(2.f * M_PI * phase);
		outputs[C4_OUTPUT_OUTPUT].setVoltage(sine);

		float frequenceQuinte = dsp::FREQ_C4 * std::pow(2.f, 7.f / 12.f);
		phase2 += frequenceQuinte * args.sampleTime;
		if (phase2 >= 1.f)
			phase2 -= 1.f;
		float sine2 = std::sin(2.f * M_PI * phase2);

		outputs[SEMITONE_OUTPUT_OUTPUT].setVoltage(sine2);
	}
};

struct C4_quinteWidget : ModuleWidget
{
	C4_quinteWidget(C4_quinte *module)
	{
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/c4_quinte.svg"),
							 asset::plugin(pluginInstance, "res/c4_quinte-dark.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(12.7, 69.268)), module, C4_quinte::C4_OUTPUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(12.7, 108.803)), module, C4_quinte::SEMITONE_OUTPUT_OUTPUT));
	}
};

Model *modelC4_quinte = createModel<C4_quinte, C4_quinteWidget>("c4_quinte");