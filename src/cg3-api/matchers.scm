(define todo '())

(define (matcher tag matcher-impl) (list tag matcher-impl))
(define matcher-tag car)
(define (matcher-impl matcher) (car (cdr matcher)))

; Matcher<Attr>... -> Matcher<Attr>
(define (attr AttrMatchers)
  (let ((matcher-impls (map matcher-impl AttrMatchers))
         (matcher-tags (map matcher-tag AttrMatchers)))
    (matcher 'Attr (mk-matcher-attr matcher-impls))))

; Matcher<CXXBaseSpecifier>... -> Matcher<CXXBaseSpecifier>
(define (cxxBaseSpecifier CXXBaseSpecifierMatchers) todo)

; Matcher<CXXCtorInitializer>... -> Matcher<CXXCtorInitializer>
(define (cxxCtorInitializer CXXCtorInitializerMatchers) todo)

; Matcher<AccessSpecDecl>... -> Matcher<Decl>
(define (accessSpecDecl AccessSpecDeclMatchers) todo)

; Matcher<BindingDecl>... -> Matcher<Decl>
(define (bindingDecl BindingDeclMatchers) todo)

; Matcher<BlockDecl>... -> Matcher<Decl>
(define (blockDecl BlockDeclMatchers) todo)

; Matcher<ClassTemplateDecl>... -> Matcher<Decl>
(define (classTemplateDecl ClassTemplateDeclMatchers) todo)

; Matcher<ClassTemplatePartialSpecializationDecl>... -> Matcher<Decl>
(define (classTemplatePartialSpecializationDecl ClassTemplatePartialSpecializationDeclMatchers) todo)

; Matcher<ClassTemplateSpecializationDecl>... -> Matcher<Decl>
(define (classTemplateSpecializationDecl ClassTemplateSpecializationDeclMatchers) todo)

; Matcher<CXXConstructorDecl>... -> Matcher<Decl>
(define (cxxConstructorDecl CXXConstructorDeclMatchers) todo)

; Matcher<CXXConversionDecl>... -> Matcher<Decl>
(define (cxxConversionDecl CXXConversionDeclMatchers) todo)

; Matcher<CXXDeductionGuideDecl>... -> Matcher<Decl>
(define (cxxDeductionGuideDecl CXXDeductionGuideDeclMatchers) todo)

; Matcher<CXXDestructorDecl>... -> Matcher<Decl>
(define (cxxDestructorDecl CXXDestructorDeclMatchers) todo)

; Matcher<CXXMethodDecl>... -> Matcher<Decl>
(define (cxxMethodDecl CXXMethodDeclMatchers) todo)

; Matcher<CXXRecordDecl>... -> Matcher<Decl>
(define (cxxRecordDecl CXXRecordDeclMatchers) todo)

; Matcher<Decl>... -> Matcher<Decl>
(define (decl DeclMatchers) todo)

; Matcher<DeclaratorDecl>... -> Matcher<Decl>
(define (declaratorDecl DeclaratorDeclMatchers) todo)

; Matcher<DecompositionDecl>... -> Matcher<Decl>
(define (decompositionDecl DecompositionDeclMatchers) todo)

; Matcher<EnumConstantDecl>... -> Matcher<Decl>
(define (enumConstantDecl EnumConstantDeclMatchers) todo)

; Matcher<EnumDecl>... -> Matcher<Decl>
(define (enumDecl EnumDeclMatchers) todo)

; Matcher<FieldDecl>... -> Matcher<Decl>
(define (fieldDecl FieldDeclMatchers) todo)

; Matcher<FriendDecl>... -> Matcher<Decl>
(define (friendDecl FriendDeclMatchers) todo)

; Matcher<FunctionDecl>... -> Matcher<Decl>
(define (functionDecl FunctionDeclMatchers) todo)

; Matcher<FunctionTemplateDecl>... -> Matcher<Decl>
(define (functionTemplateDecl FunctionTemplateDeclMatchers) todo)

; Matcher<IndirectFieldDecl>... -> Matcher<Decl>
(define (indirectFieldDecl IndirectFieldDeclMatchers) todo)

; Matcher<LabelDecl>... -> Matcher<Decl>
(define (labelDecl LabelDeclMatchers) todo)

; Matcher<LinkageSpecDecl>... -> Matcher<Decl>
(define (linkageSpecDecl LinkageSpecDeclMatchers) todo)

; Matcher<NamedDecl>... -> Matcher<Decl>
(define (namedDecl NamedDeclMatchers) todo)

; Matcher<NamespaceAliasDecl>... -> Matcher<Decl>
(define (namespaceAliasDecl NamespaceAliasDeclMatchers) todo)

; Matcher<NamespaceDecl>... -> Matcher<Decl>
(define (namespaceDecl NamespaceDeclMatchers) todo)

; Matcher<NonTypeTemplateParmDecl>... -> Matcher<Decl>
(define (nonTypeTemplateParmDecl NonTypeTemplateParmDeclMatchers) todo)

; Matcher<ObjCCategoryDecl>... -> Matcher<Decl>
(define (objcCategoryDecl ObjCCategoryDeclMatchers) todo)

; Matcher<ObjCCategoryImplDecl>... -> Matcher<Decl>
(define (objcCategoryImplDecl ObjCCategoryImplDeclMatchers) todo)

; Matcher<ObjCImplementationDecl>... -> Matcher<Decl>
(define (objcImplementationDecl ObjCImplementationDeclMatchers) todo)

; Matcher<ObjCInterfaceDecl>... -> Matcher<Decl>
(define (objcInterfaceDecl ObjCInterfaceDeclMatchers) todo)

; Matcher<ObjCIvarDecl>... -> Matcher<Decl>
(define (objcIvarDecl ObjCIvarDeclMatchers) todo)

; Matcher<ObjCMethodDecl>... -> Matcher<Decl>
(define (objcMethodDecl ObjCMethodDeclMatchers) todo)

; Matcher<ObjCPropertyDecl>... -> Matcher<Decl>
(define (objcPropertyDecl ObjCPropertyDeclMatchers) todo)

; Matcher<ObjCProtocolDecl>... -> Matcher<Decl>
(define (objcProtocolDecl ObjCProtocolDeclMatchers) todo)

; Matcher<ParmVarDecl>... -> Matcher<Decl>
(define (parmVarDecl ParmVarDeclMatchers) todo)

; Matcher<RecordDecl>... -> Matcher<Decl>
(define (recordDecl RecordDeclMatchers) todo)

; Matcher<StaticAssertDecl>... -> Matcher<Decl>
(define (staticAssertDecl StaticAssertDeclMatchers) todo)

; Matcher<TagDecl>... -> Matcher<Decl>
(define (tagDecl TagDeclMatchers) todo)

; Matcher<TemplateTemplateParmDecl>... -> Matcher<Decl>
(define (templateTemplateParmDecl TemplateTemplateParmDeclMatchers) todo)

; Matcher<TemplateTypeParmDecl>... -> Matcher<Decl>
(define (templateTypeParmDecl TemplateTypeParmDeclMatchers) todo)

; Matcher<TranslationUnitDecl>... -> Matcher<Decl>
(define (translationUnitDecl TranslationUnitDeclMatchers) todo)

; Matcher<TypeAliasDecl>... -> Matcher<Decl>
(define (typeAliasDecl TypeAliasDeclMatchers) todo)

; Matcher<TypeAliasTemplateDecl>... -> Matcher<Decl>
(define (typeAliasTemplateDecl TypeAliasTemplateDeclMatchers) todo)

; Matcher<TypedefDecl>... -> Matcher<Decl>
(define (typedefDecl TypedefDeclMatchers) todo)

; Matcher<TypedefNameDecl>... -> Matcher<Decl>
(define (typedefNameDecl TypedefNameDeclMatchers) todo)

; Matcher<UnresolvedUsingTypenameDecl>... -> Matcher<Decl>
(define (unresolvedUsingTypenameDecl UnresolvedUsingTypenameDeclMatchers) todo)

; Matcher<UnresolvedUsingValueDecl>... -> Matcher<Decl>
(define (unresolvedUsingValueDecl UnresolvedUsingValueDeclMatchers) todo)

; Matcher<UsingDecl>... -> Matcher<Decl>
(define (usingDecl UsingDeclMatchers) todo)

; Matcher<UsingDirectiveDecl>... -> Matcher<Decl>
(define (usingDirectiveDecl UsingDirectiveDeclMatchers) todo)

; Matcher<UsingEnumDecl>... -> Matcher<Decl>
(define (usingEnumDecl UsingEnumDeclMatchers) todo)

; Matcher<ValueDecl>... -> Matcher<Decl>
(define (valueDecl ValueDeclMatchers) todo)

; Matcher<VarDecl>... -> Matcher<Decl>
(define (varDecl VarDeclMatchers) todo)

; Matcher<LambdaCapture>... -> Matcher<LambdaCapture>
(define (lambdaCapture LambdaCaptureMatchers) todo)

; Matcher<NestedNameSpecifierLoc>... -> Matcher<NestedNameSpecifierLoc>
(define (nestedNameSpecifierLoc NestedNameSpecifierLocMatchers) todo)

; Matcher<NestedNameSpecifier>... -> Matcher<NestedNameSpecifier>
(define (nestedNameSpecifier NestedNameSpecifierMatchers) todo)

; Matcher<OMPDefaultClause>... -> Matcher<OMPClause>
(define (ompDefaultClause OMPDefaultClauseMatchers) todo)

; Matcher<QualType>... -> Matcher<QualType>
(define (qualType QualTypeMatchers) todo)

; Matcher<AddrLabelExpr>... -> Matcher<Stmt>
(define (addrLabelExpr AddrLabelExprMatchers) todo)

; Matcher<ArraySubscriptExpr>... -> Matcher<Stmt>
(define (arraySubscriptExpr ArraySubscriptExprMatchers) todo)

; Matcher<AsmStmt>... -> Matcher<Stmt>
(define (asmStmt AsmStmtMatchers) todo)

; Matcher<AtomicExpr>... -> Matcher<Stmt>
(define (atomicExpr AtomicExprMatchers) todo)

; Matcher<ObjCAutoreleasePoolStmt>... -> Matcher<Stmt>
(define (autoreleasePoolStmt ObjCAutoreleasePoolStmtMatchers) todo)

; Matcher<BinaryConditionalOperator>... -> Matcher<Stmt>
(define (binaryConditionalOperator BinaryConditionalOperatorMatchers) todo)

; Matcher<BinaryOperator>... -> Matcher<Stmt>
(define (binaryOperator BinaryOperatorMatchers) todo)

; Matcher<BlockExpr>... -> Matcher<Stmt>
(define (blockExpr BlockExprMatchers) todo)

; Matcher<BreakStmt>... -> Matcher<Stmt>
(define (breakStmt BreakStmtMatchers) todo)

; Matcher<CStyleCastExpr>... -> Matcher<Stmt>
(define (cStyleCastExpr CStyleCastExprMatchers) todo)

; Matcher<CallExpr>... -> Matcher<Stmt>
(define (callExpr CallExprMatchers) todo)

; Matcher<CaseStmt>... -> Matcher<Stmt>
(define (caseStmt CaseStmtMatchers) todo)

; Matcher<CastExpr>... -> Matcher<Stmt>
(define (castExpr CastExprMatchers) todo)

; Matcher<CharacterLiteral>... -> Matcher<Stmt>
(define (characterLiteral CharacterLiteralMatchers) todo)

; Matcher<ChooseExpr>... -> Matcher<Stmt>
(define (chooseExpr ChooseExprMatchers) todo)

; Matcher<CoawaitExpr>... -> Matcher<Stmt>
(define (coawaitExpr CoawaitExprMatchers) todo)

; Matcher<CompoundLiteralExpr>... -> Matcher<Stmt>
(define (compoundLiteralExpr CompoundLiteralExprMatchers) todo)

; Matcher<CompoundStmt>... -> Matcher<Stmt>
(define (compoundStmt CompoundStmtMatchers) todo)

; Matcher<ConditionalOperator>... -> Matcher<Stmt>
(define (conditionalOperator ConditionalOperatorMatchers) todo)

; Matcher<ConstantExpr>... -> Matcher<Stmt>
(define (constantExpr ConstantExprMatchers) todo)

; Matcher<ContinueStmt>... -> Matcher<Stmt>
(define (continueStmt ContinueStmtMatchers) todo)

; Matcher<CoreturnStmt>... -> Matcher<Stmt>
(define (coreturnStmt CoreturnStmtMatchers) todo)

; Matcher<CoyieldExpr>... -> Matcher<Stmt>
(define (coyieldExpr CoyieldExprMatchers) todo)

; Matcher<CUDAKernelCallExpr>... -> Matcher<Stmt>
(define (cudaKernelCallExpr CUDAKernelCallExprMatchers) todo)

; Matcher<CXXBindTemporaryExpr>... -> Matcher<Stmt>
(define (cxxBindTemporaryExpr CXXBindTemporaryExprMatchers) todo)

; Matcher<CXXBoolLiteralExpr>... -> Matcher<Stmt>
(define (cxxBoolLiteral CXXBoolLiteralExprMatchers) todo)

; Matcher<CXXCatchStmt>... -> Matcher<Stmt>
(define (cxxCatchStmt CXXCatchStmtMatchers) todo)

; Matcher<CXXConstCastExpr>... -> Matcher<Stmt>
(define (cxxConstCastExpr CXXConstCastExprMatchers) todo)

; Matcher<CXXConstructExpr>... -> Matcher<Stmt>
(define (cxxConstructExpr CXXConstructExprMatchers) todo)

; Matcher<CXXDefaultArgExpr>... -> Matcher<Stmt>
(define (cxxDefaultArgExpr CXXDefaultArgExprMatchers) todo)

; Matcher<CXXDeleteExpr>... -> Matcher<Stmt>
(define (cxxDeleteExpr CXXDeleteExprMatchers) todo)

; Matcher<CXXDependentScopeMemberExpr>... -> Matcher<Stmt>
(define (cxxDependentScopeMemberExpr CXXDependentScopeMemberExprMatchers) todo)

; Matcher<CXXDynamicCastExpr>... -> Matcher<Stmt>
(define (cxxDynamicCastExpr CXXDynamicCastExprMatchers) todo)

; Matcher<CXXForRangeStmt>... -> Matcher<Stmt>
(define (cxxForRangeStmt CXXForRangeStmtMatchers) todo)

; Matcher<CXXFunctionalCastExpr>... -> Matcher<Stmt>
(define (cxxFunctionalCastExpr CXXFunctionalCastExprMatchers) todo)

; Matcher<CXXMemberCallExpr>... -> Matcher<Stmt>
(define (cxxMemberCallExpr CXXMemberCallExprMatchers) todo)

; Matcher<CXXNewExpr>... -> Matcher<Stmt>
(define (cxxNewExpr CXXNewExprMatchers) todo)

; Matcher<CXXNoexceptExpr>... -> Matcher<Stmt>
(define (cxxNoexceptExpr CXXNoexceptExprMatchers) todo)

; Matcher<CXXNullPtrLiteralExpr>... -> Matcher<Stmt>
(define (cxxNullPtrLiteralExpr CXXNullPtrLiteralExprMatchers) todo)

; Matcher<CXXOperatorCallExpr>... -> Matcher<Stmt>
(define (cxxOperatorCallExpr CXXOperatorCallExprMatchers) todo)

; Matcher<CXXReinterpretCastExpr>... -> Matcher<Stmt>
(define (cxxReinterpretCastExpr CXXReinterpretCastExprMatchers) todo)

; Matcher<CXXRewrittenBinaryOperator>... -> Matcher<Stmt>
(define (cxxRewrittenBinaryOperator CXXRewrittenBinaryOperatorMatchers) todo)

; Matcher<CXXStaticCastExpr>... -> Matcher<Stmt>
(define (cxxStaticCastExpr CXXStaticCastExprMatchers) todo)

; Matcher<CXXStdInitializerListExpr>... -> Matcher<Stmt>
(define (cxxStdInitializerListExpr CXXStdInitializerListExprMatchers) todo)

; Matcher<CXXTemporaryObjectExpr>... -> Matcher<Stmt>
(define (cxxTemporaryObjectExpr CXXTemporaryObjectExprMatchers) todo)

; Matcher<CXXThisExpr>... -> Matcher<Stmt>
(define (cxxThisExpr CXXThisExprMatchers) todo)

; Matcher<CXXThrowExpr>... -> Matcher<Stmt>
(define (cxxThrowExpr CXXThrowExprMatchers) todo)

; Matcher<CXXTryStmt>... -> Matcher<Stmt>
(define (cxxTryStmt CXXTryStmtMatchers) todo)

; Matcher<CXXUnresolvedConstructExpr>... -> Matcher<Stmt>
(define (cxxUnresolvedConstructExpr CXXUnresolvedConstructExprMatchers) todo)

; Matcher<DeclRefExpr>... -> Matcher<Stmt>
(define (declRefExpr DeclRefExprMatchers) todo)

; Matcher<DeclStmt>... -> Matcher<Stmt>
(define (declStmt DeclStmtMatchers) todo)

; Matcher<DefaultStmt>... -> Matcher<Stmt>
(define (defaultStmt DefaultStmtMatchers) todo)

; Matcher<DependentCoawaitExpr>... -> Matcher<Stmt>
(define (dependentCoawaitExpr DependentCoawaitExprMatchers) todo)

; Matcher<DesignatedInitExpr>... -> Matcher<Stmt>
(define (designatedInitExpr DesignatedInitExprMatchers) todo)

; Matcher<DoStmt>... -> Matcher<Stmt>
(define (doStmt DoStmtMatchers) todo)

; Matcher<ExplicitCastExpr>... -> Matcher<Stmt>
(define (explicitCastExpr ExplicitCastExprMatchers) todo)

; Matcher<Expr>... -> Matcher<Stmt>
(define (expr ExprMatchers) todo)

; Matcher<ExprWithCleanups>... -> Matcher<Stmt>
(define (exprWithCleanups ExprWithCleanupsMatchers) todo)

; Matcher<FixedPointLiteral>... -> Matcher<Stmt>
(define (fixedPointLiteral FixedPointLiteralMatchers) todo)

; Matcher<FloatingLiteral>... -> Matcher<Stmt>
(define (floatLiteral FloatingLiteralMatchers) todo)

; Matcher<ForStmt>... -> Matcher<Stmt>
(define (forStmt ForStmtMatchers) todo)

; Matcher<GenericSelectionExpr>... -> Matcher<Stmt>
(define (genericSelectionExpr GenericSelectionExprMatchers) todo)

; Matcher<GNUNullExpr>... -> Matcher<Stmt>
(define (gnuNullExpr GNUNullExprMatchers) todo)

; Matcher<GotoStmt>... -> Matcher<Stmt>
(define (gotoStmt GotoStmtMatchers) todo)

; Matcher<IfStmt>... -> Matcher<Stmt>
(define (ifStmt IfStmtMatchers) todo)

; Matcher<ImaginaryLiteral>... -> Matcher<Stmt>
(define (imaginaryLiteral ImaginaryLiteralMatchers) todo)

; Matcher<ImplicitCastExpr>... -> Matcher<Stmt>
(define (implicitCastExpr ImplicitCastExprMatchers) todo)

; Matcher<ImplicitValueInitExpr>... -> Matcher<Stmt>
(define (implicitValueInitExpr ImplicitValueInitExprMatchers) todo)

; Matcher<InitListExpr>... -> Matcher<Stmt>
(define (initListExpr InitListExprMatchers) todo)

; Matcher<IntegerLiteral>... -> Matcher<Stmt>
(define (integerLiteral IntegerLiteralMatchers) todo)

; Matcher<LabelStmt>... -> Matcher<Stmt>
(define (labelStmt LabelStmtMatchers) todo)

; Matcher<LambdaExpr>... -> Matcher<Stmt>
(define (lambdaExpr LambdaExprMatchers) todo)

; Matcher<MaterializeTemporaryExpr>... -> Matcher<Stmt>
(define (materializeTemporaryExpr MaterializeTemporaryExprMatchers) todo)

; Matcher<MemberExpr>... -> Matcher<Stmt>
(define (memberExpr MemberExprMatchers) todo)

; Matcher<NullStmt>... -> Matcher<Stmt>
(define (nullStmt NullStmtMatchers) todo)

; Matcher<ObjCAtCatchStmt>... -> Matcher<Stmt>
(define (objcCatchStmt ObjCAtCatchStmtMatchers) todo)

; Matcher<ObjCAtFinallyStmt>... -> Matcher<Stmt>
(define (objcFinallyStmt ObjCAtFinallyStmtMatchers) todo)

; Matcher<ObjCIvarRefExpr>... -> Matcher<Stmt>
(define (objcIvarRefExpr ObjCIvarRefExprMatchers) todo)

; Matcher<ObjCMessageExpr>... -> Matcher<Stmt>
(define (objcMessageExpr ObjCMessageExprMatchers) todo)

; Matcher<ObjCStringLiteral>... -> Matcher<Stmt>
(define (objcStringLiteral ObjCStringLiteralMatchers) todo)

; Matcher<ObjCAtThrowStmt>... -> Matcher<Stmt>
(define (objcThrowStmt ObjCAtThrowStmtMatchers) todo)

; Matcher<ObjCAtTryStmt>... -> Matcher<Stmt>
(define (objcTryStmt ObjCAtTryStmtMatchers) todo)

; Matcher<OMPExecutableDirective>... -> Matcher<Stmt>
(define (ompExecutableDirective OMPExecutableDirectiveMatchers) todo)

; Matcher<OpaqueValueExpr>... -> Matcher<Stmt>
(define (opaqueValueExpr OpaqueValueExprMatchers) todo)

; Matcher<ParenExpr>... -> Matcher<Stmt>
(define (parenExpr ParenExprMatchers) todo)

; Matcher<ParenListExpr>... -> Matcher<Stmt>
(define (parenListExpr ParenListExprMatchers) todo)

; Matcher<PredefinedExpr>... -> Matcher<Stmt>
(define (predefinedExpr PredefinedExprMatchers) todo)

; Matcher<ReturnStmt>... -> Matcher<Stmt>
(define (returnStmt ReturnStmtMatchers) todo)

; Matcher<Stmt>... -> Matcher<Stmt>
(define (stmt StmtMatchers) todo)

; Matcher<StmtExpr>... -> Matcher<Stmt>
(define (stmtExpr StmtExprMatchers) todo)

; Matcher<StringLiteral>... -> Matcher<Stmt>
(define (stringLiteral StringLiteralMatchers) todo)

; Matcher<SubstNonTypeTemplateParmExpr>... -> Matcher<Stmt>
(define (substNonTypeTemplateParmExpr SubstNonTypeTemplateParmExprMatchers) todo)

; Matcher<SwitchCase>... -> Matcher<Stmt>
(define (switchCase SwitchCaseMatchers) todo)

; Matcher<SwitchStmt>... -> Matcher<Stmt>
(define (switchStmt SwitchStmtMatchers) todo)

; Matcher<UnaryExprOrTypeTraitExpr>... -> Matcher<Stmt>
(define (unaryExprOrTypeTraitExpr UnaryExprOrTypeTraitExprMatchers) todo)

; Matcher<UnaryOperator>... -> Matcher<Stmt>
(define (unaryOperator UnaryOperatorMatchers) todo)

; Matcher<UnresolvedLookupExpr>... -> Matcher<Stmt>
(define (unresolvedLookupExpr UnresolvedLookupExprMatchers) todo)

; Matcher<UnresolvedMemberExpr>... -> Matcher<Stmt>
(define (unresolvedMemberExpr UnresolvedMemberExprMatchers) todo)

; Matcher<UserDefinedLiteral>... -> Matcher<Stmt>
(define (userDefinedLiteral UserDefinedLiteralMatchers) todo)

; Matcher<WhileStmt>... -> Matcher<Stmt>
(define (whileStmt WhileStmtMatchers) todo)

; Matcher<TemplateArgumentLoc>... -> Matcher<TemplateArgumentLoc>
(define (templateArgumentLoc TemplateArgumentLocMatchers) todo)

; Matcher<TemplateArgument>... -> Matcher<TemplateArgument>
(define (templateArgument TemplateArgumentMatchers) todo)

; Matcher<TemplateName>... -> Matcher<TemplateName>
(define (templateName TemplateNameMatchers) todo)

; Matcher<ElaboratedTypeLoc>... -> Matcher<TypeLoc>
(define (elaboratedTypeLoc ElaboratedTypeLocMatchers) todo)

; Matcher<PointerTypeLoc>... -> Matcher<TypeLoc>
(define (pointerTypeLoc PointerTypeLocMatchers) todo)

; Matcher<QualifiedTypeLoc>... -> Matcher<TypeLoc>
(define (qualifiedTypeLoc QualifiedTypeLocMatchers) todo)

; Matcher<ReferenceTypeLoc>... -> Matcher<TypeLoc>
(define (referenceTypeLoc ReferenceTypeLocMatchers) todo)

; Matcher<TemplateSpecializationTypeLoc>... -> Matcher<TypeLoc>
(define (templateSpecializationTypeLoc TemplateSpecializationTypeLocMatchers) todo)

; Matcher<TypeLoc>... -> Matcher<TypeLoc>
(define (typeLoc TypeLocMatchers) todo)

; Matcher<ArrayType>... -> Matcher<Type>
(define (arrayType ArrayTypeMatchers) todo)

; Matcher<AtomicType>... -> Matcher<Type>
(define (atomicType AtomicTypeMatchers) todo)

; Matcher<AutoType>... -> Matcher<Type>
(define (autoType AutoTypeMatchers) todo)

; Matcher<BlockPointerType>... -> Matcher<Type>
(define (blockPointerType BlockPointerTypeMatchers) todo)

; Matcher<BuiltinType>... -> Matcher<Type>
(define (builtinType BuiltinTypeMatchers) todo)

; Matcher<ComplexType>... -> Matcher<Type>
(define (complexType ComplexTypeMatchers) todo)

; Matcher<ConstantArrayType>... -> Matcher<Type>
(define (constantArrayType ConstantArrayTypeMatchers) todo)

; Matcher<DecayedType>... -> Matcher<Type>
(define (decayedType DecayedTypeMatchers) todo)

; Matcher<DecltypeType>... -> Matcher<Type>
(define (decltypeType DecltypeTypeMatchers) todo)

; Matcher<DeducedTemplateSpecializationType>... -> Matcher<Type>
(define (deducedTemplateSpecializationType DeducedTemplateSpecializationTypeMatchers) todo)

; Matcher<DependentSizedArrayType>... -> Matcher<Type>
(define (dependentSizedArrayType DependentSizedArrayTypeMatchers) todo)

; Matcher<ElaboratedType>... -> Matcher<Type>
(define (elaboratedType ElaboratedTypeMatchers) todo)

; Matcher<EnumType>... -> Matcher<Type>
(define (enumType EnumTypeMatchers) todo)

; Matcher<FunctionProtoType>... -> Matcher<Type>
(define (functionProtoType FunctionProtoTypeMatchers) todo)

; Matcher<FunctionType>... -> Matcher<Type>
(define (functionType FunctionTypeMatchers) todo)

; Matcher<IncompleteArrayType>... -> Matcher<Type>
(define (incompleteArrayType IncompleteArrayTypeMatchers) todo)

; Matcher<InjectedClassNameType>... -> Matcher<Type>
(define (injectedClassNameType InjectedClassNameTypeMatchers) todo)

; Matcher<LValueReferenceType>... -> Matcher<Type>
(define (lValueReferenceType LValueReferenceTypeMatchers) todo)

; Matcher<MemberPointerType>... -> Matcher<Type>
(define (memberPointerType MemberPointerTypeMatchers) todo)

; Matcher<ObjCObjectPointerType>... -> Matcher<Type>
(define (objcObjectPointerType ObjCObjectPointerTypeMatchers) todo)

; Matcher<ParenType>... -> Matcher<Type>
(define (parenType ParenTypeMatchers) todo)

; Matcher<PointerType>... -> Matcher<Type>
(define (pointerType PointerTypeMatchers) todo)

; Matcher<RValueReferenceType>... -> Matcher<Type>
(define (rValueReferenceType RValueReferenceTypeMatchers) todo)

; Matcher<RecordType>... -> Matcher<Type>
(define (recordType RecordTypeMatchers) todo)

; Matcher<ReferenceType>... -> Matcher<Type>
(define (referenceType ReferenceTypeMatchers) todo)

; Matcher<SubstTemplateTypeParmType>... -> Matcher<Type>
(define (substTemplateTypeParmType SubstTemplateTypeParmTypeMatchers) todo)

; Matcher<TagType>... -> Matcher<Type>
(define (tagType TagTypeMatchers) todo)

; Matcher<TemplateSpecializationType>... -> Matcher<Type>
(define (templateSpecializationType TemplateSpecializationTypeMatchers) todo)

; Matcher<TemplateTypeParmType>... -> Matcher<Type>
(define (templateTypeParmType TemplateTypeParmTypeMatchers) todo)

; Matcher<Type>... -> Matcher<Type>
(define (type TypeMatchers) todo)

; Matcher<TypedefType>... -> Matcher<Type>
(define (typedefType TypedefTypeMatchers) todo)

; Matcher<UnaryTransformType>... -> Matcher<Type>
(define (unaryTransformType UnaryTransformTypeMatchers) todo)

; Matcher<UsingType>... -> Matcher<Type>
(define (usingType UsingTypeMatchers) todo)

; Matcher<VariableArrayType>... -> Matcher<Type>
(define (variableArrayType VariableArrayTypeMatchers) todo)


; Matcher<*>, ..., Matcher<*> -> Matcher<*>
(define (allOf *Matchers) todo)

; Matcher<*>, ..., Matcher<*> -> Matcher<*>
(define (anyOf *Matchers) todo)

; () -> Matcher<*>
(define (anything) todo)

; nodeMatcherFunction... -> unspecified
(define (mapAnyOf nodeMatcherFunctionParams) todo)

; Matcher<*> -> Matcher<*>
(define (unless MatcherParams) todo)

; () -> Matcher<Attr>
(define (isImplicit) todo)

; StringRef, ..., StringRef -> Matcher<BinaryOperator>
(define (hasAnyOperatorName StringRefParams) todo)

; std::string Name -> Matcher<BinaryOperator>
(define (hasOperatorName stdParams) todo)

; () -> Matcher<BinaryOperator>
(define (isAssignmentOperator) todo)

; () -> Matcher<BinaryOperator>
(define (isComparisonOperator) todo)

; () -> Matcher<CXXBaseSpecifier>
(define (isPrivate) todo)

; () -> Matcher<CXXBaseSpecifier>
(define (isProtected) todo)

; () -> Matcher<CXXBaseSpecifier>
(define (isPublic) todo)

; () -> Matcher<CXXBaseSpecifier>
(define (isVirtual) todo)

; bool Value -> Matcher<CXXBoolLiteralExpr>
(define (equals boolParams) todo)

; const ValueT Value -> Matcher<CXXBoolLiteralExpr>
(define (equals constParams) todo)

; double Value -> Matcher<CXXBoolLiteralExpr>
(define (equals doubleParams) todo)

; unsigned Value -> Matcher<CXXBoolLiteralExpr>
(define (equals unsignedParams) todo)

; () -> Matcher<CXXCatchStmt>
(define (isCatchAll) todo)

; unsigned N -> Matcher<CXXConstructExpr>
(define (argumentCountIs unsignedParams) todo)

; () -> Matcher<CXXConstructExpr>
(define (isListInitialization) todo)

; () -> Matcher<CXXConstructExpr>
(define (requiresZeroInitialization) todo)

; () -> Matcher<CXXConstructorDecl>
(define (isCopyConstructor) todo)

; () -> Matcher<CXXConstructorDecl>
(define (isDefaultConstructor) todo)

; () -> Matcher<CXXConstructorDecl>
(define (isDelegatingConstructor) todo)

; () -> Matcher<CXXConstructorDecl>
(define (isExplicit) todo)

; () -> Matcher<CXXConstructorDecl>
(define (isInheritingConstructor) todo)

; () -> Matcher<CXXConstructorDecl>
(define (isMoveConstructor) todo)

; () -> Matcher<CXXConversionDecl>
(define (isExplicit) todo)

; () -> Matcher<CXXCtorInitializer>
(define (isBaseInitializer) todo)

; () -> Matcher<CXXCtorInitializer>
(define (isMemberInitializer) todo)

; () -> Matcher<CXXCtorInitializer>
(define (isWritten) todo)

; () -> Matcher<CXXDeductionGuideDecl>
(define (isExplicit) todo)

; std::string N -> Matcher<CXXDependentScopeMemberExpr>
(define (hasMemberName stdParams) todo)

; () -> Matcher<CXXDependentScopeMemberExpr>
(define (isArrow) todo)

; std::string BindingID -> Matcher<CXXDependentScopeMemberExpr>
(define (memberHasSameNameAsBoundNode stdParams) todo)

; () -> Matcher<CXXMethodDecl>
(define (isConst) todo)

; () -> Matcher<CXXMethodDecl>
(define (isCopyAssignmentOperator) todo)

; () -> Matcher<CXXMethodDecl>
(define (isFinal) todo)

; () -> Matcher<CXXMethodDecl>
(define (isMoveAssignmentOperator) todo)

; () -> Matcher<CXXMethodDecl>
(define (isOverride) todo)

; () -> Matcher<CXXMethodDecl>
(define (isPure) todo)

; () -> Matcher<CXXMethodDecl>
(define (isUserProvided) todo)

; () -> Matcher<CXXMethodDecl>
(define (isVirtual) todo)

; () -> Matcher<CXXMethodDecl>
(define (isVirtualAsWritten) todo)

; () -> Matcher<CXXNewExpr>
(define (isArray) todo)

; StringRef, ..., StringRef -> Matcher<CXXOperatorCallExpr>
(define (hasAnyOperatorName StringRefParams) todo)

; StringRef, ..., StringRef -> Matcher<CXXOperatorCallExpr>
(define (hasAnyOverloadedOperatorName StringRefParams) todo)

; std::string Name -> Matcher<CXXOperatorCallExpr>
(define (hasOperatorName stdParams) todo)

; StringRef Name -> Matcher<CXXOperatorCallExpr>
(define (hasOverloadedOperatorName StringRefParams) todo)

; () -> Matcher<CXXOperatorCallExpr>
(define (isAssignmentOperator) todo)

; () -> Matcher<CXXOperatorCallExpr>
(define (isComparisonOperator) todo)

; () -> Matcher<CXXRecordDecl>
(define (hasDefinition) todo)

; std::string BaseName -> Matcher<CXXRecordDecl>
(define (isDerivedFrom stdParams) todo)

; std::string BaseName -> Matcher<CXXRecordDecl>
(define (isDirectlyDerivedFrom stdParams) todo)

; () -> Matcher<CXXRecordDecl>
(define (isExplicitTemplateSpecialization) todo)

; () -> Matcher<CXXRecordDecl>
(define (isFinal) todo)

; () -> Matcher<CXXRecordDecl>
(define (isLambda) todo)

; std::string BaseName -> Matcher<CXXRecordDecl>
(define (isSameOrDerivedFrom stdParams) todo)

; () -> Matcher<CXXRecordDecl>
(define (isTemplateInstantiation) todo)

; StringRef, ..., StringRef -> Matcher<CXXRewrittenBinaryOperator>
(define (hasAnyOperatorName StringRefParams) todo)

; std::string Name -> Matcher<CXXRewrittenBinaryOperator>
(define (hasOperatorName stdParams) todo)

; () -> Matcher<CXXRewrittenBinaryOperator>
(define (isAssignmentOperator) todo)

; () -> Matcher<CXXRewrittenBinaryOperator>
(define (isComparisonOperator) todo)

; unsigned N -> Matcher<CXXUnresolvedConstructExpr>
(define (argumentCountIs unsignedParams) todo)

; unsigned N -> Matcher<CallExpr>
(define (argumentCountIs unsignedParams) todo)

; () -> Matcher<CallExpr>
(define (usesADL) todo)

; CastKind Kind -> Matcher<CastExpr>
(define (hasCastKind CastKindParams) todo)

; bool Value -> Matcher<CharacterLiteral>
(define (equals boolParams) todo)

; const ValueT Value -> Matcher<CharacterLiteral>
(define (equals constParams) todo)

; double Value -> Matcher<CharacterLiteral>
(define (equals doubleParams) todo)

; unsigned Value -> Matcher<CharacterLiteral>
(define (equals unsignedParams) todo)

; unsigned N -> Matcher<ClassTemplateSpecializationDecl>
(define (templateArgumentCountIs unsignedParams) todo)

; unsigned N -> Matcher<CompoundStmt>
(define (statementCountIs unsignedParams) todo)

; unsigned N -> Matcher<ConstantArrayType>
(define (hasSize unsignedParams) todo)

; unsigned N -> Matcher<DeclStmt>
(define (declCountIs unsignedParams) todo)

; std::string ID -> Matcher<Decl>
(define (equalsBoundNode stdParams) todo)

; const Decl* Other -> Matcher<Decl>
(define (equalsNode constParams) todo)

; attr::Kind AttrKind -> Matcher<Decl>
(define (hasAttr attrParams) todo)

; std::string MacroName -> Matcher<Decl>
(define (isExpandedFromMacro stdParams) todo)

; StringRef RegExp, Regex::RegexFlags Flags = NoFlags -> Matcher<Decl>
(define (isExpansionInFileMatching StringRefParams) todo)

; () -> Matcher<Decl>
(define (isExpansionInMainFile) todo)

; () -> Matcher<Decl>
(define (isExpansionInSystemHeader) todo)

; () -> Matcher<Decl>
(define (isImplicit) todo)

; () -> Matcher<Decl>
(define (isInAnonymousNamespace) todo)

; () -> Matcher<Decl>
(define (isInStdNamespace) todo)

; () -> Matcher<Decl>
(define (isInstantiated) todo)

; () -> Matcher<Decl>
(define (isPrivate) todo)

; () -> Matcher<Decl>
(define (isProtected) todo)

; () -> Matcher<Decl>
(define (isPublic) todo)

; unsigned N -> Matcher<DesignatedInitExpr>
(define (designatorCountIs unsignedParams) todo)

; () -> Matcher<EnumDecl>
(define (isScoped) todo)

; () -> Matcher<Expr>
(define (isInstantiationDependent) todo)

; () -> Matcher<Expr>
(define (isTypeDependent) todo)

; () -> Matcher<Expr>
(define (isValueDependent) todo)

; () -> Matcher<Expr>
(define (nullPointerConstant) todo)

; unsigned Width -> Matcher<FieldDecl>
(define (hasBitWidth unsignedParams) todo)

; () -> Matcher<FieldDecl>
(define (isBitField) todo)

; const ValueT Value -> Matcher<FloatingLiteral>
(define (equals constParams) todo)

; double Value -> Matcher<FloatingLiteral>
(define (equals doubleParams) todo)

; StringRef, ..., StringRef -> Matcher<FunctionDecl>
(define (hasAnyOverloadedOperatorName StringRefParams) todo)

; () -> Matcher<FunctionDecl>
(define (hasDynamicExceptionSpec) todo)

; StringRef Name -> Matcher<FunctionDecl>
(define (hasOverloadedOperatorName StringRefParams) todo)

; () -> Matcher<FunctionDecl>
(define (hasTrailingReturn) todo)

; () -> Matcher<FunctionDecl>
(define (isConsteval) todo)

; () -> Matcher<FunctionDecl>
(define (isConstexpr) todo)

; () -> Matcher<FunctionDecl>
(define (isDefaulted) todo)

; () -> Matcher<FunctionDecl>
(define (isDefinition) todo)

; () -> Matcher<FunctionDecl>
(define (isDeleted) todo)

; () -> Matcher<FunctionDecl>
(define (isExplicitTemplateSpecialization) todo)

; () -> Matcher<FunctionDecl>
(define (isExternC) todo)

; () -> Matcher<FunctionDecl>
(define (isInline) todo)

; () -> Matcher<FunctionDecl>
(define (isMain) todo)

; () -> Matcher<FunctionDecl>
(define (isNoReturn) todo)

; () -> Matcher<FunctionDecl>
(define (isNoThrow) todo)

; () -> Matcher<FunctionDecl>
(define (isStaticStorageClass) todo)

; () -> Matcher<FunctionDecl>
(define (isTemplateInstantiation) todo)

; () -> Matcher<FunctionDecl>
(define (isVariadic) todo)

; () -> Matcher<FunctionDecl>
(define (isWeak) todo)

; unsigned N -> Matcher<FunctionDecl>
(define (parameterCountIs unsignedParams) todo)

; () -> Matcher<FunctionProtoType>
(define (hasDynamicExceptionSpec) todo)

; () -> Matcher<FunctionProtoType>
(define (isNoThrow) todo)

; unsigned N -> Matcher<FunctionProtoType>
(define (parameterCountIs unsignedParams) todo)

; () -> Matcher<IfStmt>
(define (isConsteval) todo)

; () -> Matcher<IfStmt>
(define (isConstexpr) todo)

; bool Value -> Matcher<IntegerLiteral>
(define (equals boolParams) todo)

; const ValueT Value -> Matcher<IntegerLiteral>
(define (equals constParams) todo)

; double Value -> Matcher<IntegerLiteral>
(define (equals doubleParams) todo)

; unsigned Value -> Matcher<IntegerLiteral>
(define (equals unsignedParams) todo)

; () -> Matcher<LambdaCapture>
(define (capturesThis) todo)

; () -> Matcher<LambdaCapture>
(define (isImplicit) todo)

; () -> Matcher<MemberExpr>
(define (isArrow) todo)

; StringRef, ..., StringRef -> Matcher<NamedDecl>
(define (hasAnyName StringRefParams) todo)

; () -> Matcher<NamedDecl>
(define (hasExternalFormalLinkage) todo)

; StringRef Name -> Matcher<NamedDecl>
(define (hasName StringRefParams) todo)

; StringRef RegExp, Regex::RegexFlags Flags = NoFlags -> Matcher<NamedDecl>
(define (matchesName StringRefParams) todo)

; () -> Matcher<NamespaceDecl>
(define (isAnonymous) todo)

; () -> Matcher<NamespaceDecl>
(define (isInline) todo)

; () -> Matcher<OMPDefaultClause>
(define (isFirstPrivateKind) todo)

; () -> Matcher<OMPDefaultClause>
(define (isNoneKind) todo)

; () -> Matcher<OMPDefaultClause>
(define (isPrivateKind) todo)

; () -> Matcher<OMPDefaultClause>
(define (isSharedKind) todo)

; OpenMPClauseKind CKind -> Matcher<OMPExecutableDirective>
(define (isAllowedToContainClauseKind OpenMPClauseKindParams) todo)

; () -> Matcher<OMPExecutableDirective>
(define (isStandaloneDirective) todo)

; std::string BaseName -> Matcher<ObjCInterfaceDecl>
(define (isDerivedFrom stdParams) todo)

; std::string BaseName -> Matcher<ObjCInterfaceDecl>
(define (isDirectlyDerivedFrom stdParams) todo)

; std::string BaseName -> Matcher<ObjCInterfaceDecl>
(define (isSameOrDerivedFrom stdParams) todo)

; unsigned N -> Matcher<ObjCMessageExpr>
(define (argumentCountIs unsignedParams) todo)

; StringRef, ..., StringRef -> Matcher<ObjCMessageExpr>
(define (hasAnySelector StringRefParams) todo)

; () -> Matcher<ObjCMessageExpr>
(define (hasKeywordSelector) todo)

; () -> Matcher<ObjCMessageExpr>
(define (hasNullSelector) todo)

; std::string BaseName -> Matcher<ObjCMessageExpr>
(define (hasSelector stdParams) todo)

; () -> Matcher<ObjCMessageExpr>
(define (hasUnarySelector) todo)

; () -> Matcher<ObjCMessageExpr>
(define (isClassMessage) todo)

; () -> Matcher<ObjCMessageExpr>
(define (isInstanceMessage) todo)

; StringRef RegExp, Regex::RegexFlags Flags = NoFlags -> Matcher<ObjCMessageExpr>
(define (matchesSelector StringRefParams) todo)

; unsigned N -> Matcher<ObjCMessageExpr>
(define (numSelectorArgs unsignedParams) todo)

; () -> Matcher<ObjCMethodDecl>
(define (isClassMethod) todo)

; () -> Matcher<ObjCMethodDecl>
(define (isDefinition) todo)

; () -> Matcher<ObjCMethodDecl>
(define (isInstanceMethod) todo)

; () -> Matcher<ParmVarDecl>
(define (hasDefaultArgument) todo)

; unsigned N -> Matcher<ParmVarDecl>
(define (isAtPosition unsignedParams) todo)

; std::string Name -> Matcher<QualType>
(define (asString stdParams) todo)

; std::string ID -> Matcher<QualType>
(define (equalsBoundNode stdParams) todo)

; () -> Matcher<QualType>
(define (hasLocalQualifiers) todo)

; () -> Matcher<QualType>
(define (isAnyCharacter) todo)

; () -> Matcher<QualType>
(define (isAnyPointer) todo)

; () -> Matcher<QualType>
(define (isConstQualified) todo)

; () -> Matcher<QualType>
(define (isInteger) todo)

; () -> Matcher<QualType>
(define (isSignedInteger) todo)

; () -> Matcher<QualType>
(define (isUnsignedInteger) todo)

; () -> Matcher<QualType>
(define (isVolatileQualified) todo)

; std::string ID -> Matcher<Stmt>
(define (equalsBoundNode stdParams) todo)

; const Stmt* Other -> Matcher<Stmt>
(define (equalsNode constParams) todo)

; std::string MacroName -> Matcher<Stmt>
(define (isExpandedFromMacro stdParams) todo)

; StringRef RegExp, Regex::RegexFlags Flags = NoFlags -> Matcher<Stmt>
(define (isExpansionInFileMatching StringRefParams) todo)

; () -> Matcher<Stmt>
(define (isExpansionInMainFile) todo)

; () -> Matcher<Stmt>
(define (isExpansionInSystemHeader) todo)

; () -> Matcher<Stmt>
(define (isInTemplateInstantiation) todo)

; unsigned N -> Matcher<StringLiteral>
(define (hasSize unsignedParams) todo)

; () -> Matcher<TagDecl>
(define (isClass) todo)

; () -> Matcher<TagDecl>
(define (isDefinition) todo)

; () -> Matcher<TagDecl>
(define (isEnum) todo)

; () -> Matcher<TagDecl>
(define (isStruct) todo)

; () -> Matcher<TagDecl>
(define (isUnion) todo)

; std::string Value -> Matcher<TemplateArgument>
(define (equalsIntegralValue stdParams) todo)

; () -> Matcher<TemplateArgument>
(define (isIntegral) todo)

; unsigned N -> Matcher<TemplateSpecializationType>
(define (templateArgumentCountIs unsignedParams) todo)

; std::string MacroName -> Matcher<TypeLoc>
(define (isExpandedFromMacro stdParams) todo)

; StringRef RegExp, Regex::RegexFlags Flags = NoFlags -> Matcher<TypeLoc>
(define (isExpansionInFileMatching StringRefParams) todo)

; () -> Matcher<TypeLoc>
(define (isExpansionInMainFile) todo)

; () -> Matcher<TypeLoc>
(define (isExpansionInSystemHeader) todo)

; () -> Matcher<Type>
(define (booleanType) todo)

; std::string ID -> Matcher<Type>
(define (equalsBoundNode stdParams) todo)

; const Type* Other -> Matcher<Type>
(define (equalsNode constParams) todo)

; () -> Matcher<Type>
(define (realFloatingPointType) todo)

; () -> Matcher<Type>
(define (voidType) todo)

; UnaryExprOrTypeTrait Kind -> Matcher<UnaryExprOrTypeTraitExpr>
(define (ofKind UnaryExprOrTypeTraitParams) todo)

; StringRef, ..., StringRef -> Matcher<UnaryOperator>
(define (hasAnyOperatorName StringRefParams) todo)

; std::string Name -> Matcher<UnaryOperator>
(define (hasOperatorName stdParams) todo)

; () -> Matcher<UnresolvedMemberExpr>
(define (isArrow) todo)

; () -> Matcher<VarDecl>
(define (hasAutomaticStorageDuration) todo)

; () -> Matcher<VarDecl>
(define (hasGlobalStorage) todo)

; () -> Matcher<VarDecl>
(define (hasLocalStorage) todo)

; () -> Matcher<VarDecl>
(define (hasStaticStorageDuration) todo)

; () -> Matcher<VarDecl>
(define (hasThreadStorageDuration) todo)

; () -> Matcher<VarDecl>
(define (isConstexpr) todo)

; () -> Matcher<VarDecl>
(define (isConstinit) todo)

; () -> Matcher<VarDecl>
(define (isDefinition) todo)

; () -> Matcher<VarDecl>
(define (isExceptionVariable) todo)

; () -> Matcher<VarDecl>
(define (isExplicitTemplateSpecialization) todo)

; () -> Matcher<VarDecl>
(define (isExternC) todo)

; () -> Matcher<VarDecl>
(define (isInitCapture) todo)

; () -> Matcher<VarDecl>
(define (isInline) todo)

; () -> Matcher<VarDecl>
(define (isStaticLocal) todo)

; () -> Matcher<VarDecl>
(define (isStaticStorageClass) todo)

; () -> Matcher<VarDecl>
(define (isTemplateInstantiation) todo)


; Matcher<*>...Matcher<*> -> Matcher<*>
(define (binaryOperation *Matchers) todo)

; Matcher<*>, ..., Matcher<*> -> Matcher<*>
(define (eachOf *Matchers) todo)

; Matcher<*> Matcher -> Matcher<*>
(define (findAll *Matchers) todo)

; Matcher<*> -> Matcher<*>
(define (forEachDescendant MatcherParams) todo)

; Matcher<*> -> Matcher<*>
(define (forEach MatcherParams) todo)

; Matcher<*> -> Matcher<*>
(define (hasAncestor MatcherParams) todo)

; Matcher<*> -> Matcher<*>
(define (hasDescendant MatcherParams) todo)

; Matcher<*> -> Matcher<*>
(define (has MatcherParams) todo)

; Matcher<*> -> Matcher<*>
(define (hasParent MatcherParams) todo)

; Matcher<*>...Matcher<*> -> Matcher<*>
(define (invocation *Matchers) todo)

; Matcher<*> -> Matcher<*>
(define (optionally MatcherParams) todo)

; TraversalKind TK, Matcher<*> InnerMatcher -> Matcher<*>
(define (traverse TraversalKindParams) todo)

; Matcher<Expr> InnerMatcher -> Matcher<AbstractConditionalOperator>
(define (hasCondition ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<AbstractConditionalOperator>
(define (hasFalseExpression ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<AbstractConditionalOperator>
(define (hasTrueExpression ExprMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<AddrLabelExpr>
(define (hasDeclaration DeclMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<ArraySubscriptExpr>
(define (hasBase ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<ArraySubscriptExpr>
(define (hasIndex ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<ArraySubscriptExpr>
(define (hasLHS ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<ArraySubscriptExpr>
(define (hasRHS ExprMatchers) todo)

; Matcher<Type> -> Matcher<ArrayType>
(define (hasElementType MatcherParams) todo)

; Matcher<Type> -> Matcher<AtomicType>
(define (hasValueType MatcherParams) todo)

; Matcher<Type> -> Matcher<AutoType>
(define (hasDeducedType MatcherParams) todo)

; Matcher<UsingShadowDecl> InnerMatcher -> Matcher<BaseUsingDecl>
(define (hasAnyUsingShadowDecl UsingShadowDeclMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<BinaryOperator>
(define (hasEitherOperand ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<BinaryOperator>
(define (hasLHS ExprMatchers) todo)

; Matcher<Expr> Matcher1, Matcher<Expr> Matcher2 -> Matcher<BinaryOperator>
(define (hasOperands Expr> Matcher1 ,Matcher<ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<BinaryOperator>
(define (hasRHS ExprMatchers) todo)

; Matcher<ValueDecl> InnerMatcher -> Matcher<BindingDecl>
(define (forDecomposition ValueDeclMatchers) todo)

; Matcher<ParmVarDecl> InnerMatcher -> Matcher<BlockDecl>
(define (hasAnyParameter ParmVarDeclMatchers) todo)

; unsigned N, Matcher<ParmVarDecl> InnerMatcher -> Matcher<BlockDecl>
(define (hasParameter unsignedParams) todo)

; Matcher<TypeLoc> Inner -> Matcher<BlockDecl>
(define (hasTypeLoc TypeLocMatchers) todo)

; Matcher<Type> -> Matcher<BlockPointerType>
(define (pointee MatcherParams) todo)

; Matcher<TypeLoc> Inner -> Matcher<CXXBaseSpecifier>
(define (hasTypeLoc TypeLocMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<CXXBaseSpecifier>
(define (hasType DeclMatchers) todo)

; Matcher<QualType> InnerMatcher -> Matcher<CXXBaseSpecifier>
(define (hasType QualTypeMatchers) todo)

; Matcher<Expr> ArgMatcher, Matcher<ParmVarDecl> ParamMatcher -> Matcher<CXXConstructExpr>
(define (forEachArgumentWithParam Expr> ArgMatcher ,Matcher<ParmVarDeclMatchers) todo)

; Matcher<Expr> ArgMatcher, Matcher<QualType> ParamMatcher -> Matcher<CXXConstructExpr>
(define (forEachArgumentWithParamType Expr> ArgMatcher ,Matcher<QualTypeMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CXXConstructExpr>
(define (hasAnyArgument ExprMatchers) todo)

; unsigned N, Matcher<Expr> InnerMatcher -> Matcher<CXXConstructExpr>
(define (hasArgument unsignedParams) todo)

; Matcher<Decl> InnerMatcher -> Matcher<CXXConstructExpr>
(define (hasDeclaration DeclMatchers) todo)

; Matcher<CXXCtorInitializer> InnerMatcher -> Matcher<CXXConstructorDecl>
(define (forEachConstructorInitializer CXXCtorInitializerMatchers) todo)

; Matcher<CXXCtorInitializer> InnerMatcher -> Matcher<CXXConstructorDecl>
(define (hasAnyConstructorInitializer CXXCtorInitializerMatchers) todo)

; Matcher<FieldDecl> InnerMatcher -> Matcher<CXXCtorInitializer>
(define (forField FieldDeclMatchers) todo)

; Matcher<TypeLoc> Inner -> Matcher<CXXCtorInitializer>
(define (hasTypeLoc TypeLocMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CXXCtorInitializer>
(define (withInitializer ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CXXDependentScopeMemberExpr>
(define (hasObjectExpression ExprMatchers) todo)

; Matcher<Stmt> InnerMatcher -> Matcher<CXXForRangeStmt>
(define (hasBody StmtMatchers) todo)

; Matcher<Stmt> InnerMatcher -> Matcher<CXXForRangeStmt>
(define (hasInitStatement StmtMatchers) todo)

; Matcher<VarDecl> InnerMatcher -> Matcher<CXXForRangeStmt>
(define (hasLoopVariable VarDeclMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CXXForRangeStmt>
(define (hasRangeInit ExprMatchers) todo)

; Matcher<TypeLoc> Inner -> Matcher<CXXFunctionalCastExpr>
(define (hasTypeLoc TypeLocMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CXXMemberCallExpr>
(define (onImplicitObjectArgument ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CXXMemberCallExpr>
(define (on ExprMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<CXXMemberCallExpr>
(define (thisPointerType DeclMatchers) todo)

; Matcher<QualType> InnerMatcher -> Matcher<CXXMemberCallExpr>
(define (thisPointerType QualTypeMatchers) todo)

; Matcher<CXXMethodDecl> InnerMatcher -> Matcher<CXXMethodDecl>
(define (forEachOverridden CXXMethodDeclMatchers) todo)

; Matcher<CXXRecordDecl> InnerMatcher -> Matcher<CXXMethodDecl>
(define (ofClass CXXRecordDeclMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CXXNewExpr>
(define (hasAnyPlacementArg ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CXXNewExpr>
(define (hasArraySize ExprMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<CXXNewExpr>
(define (hasDeclaration DeclMatchers) todo)

; unsigned Index, Matcher<Expr> InnerMatcher -> Matcher<CXXNewExpr>
(define (hasPlacementArg unsignedParams) todo)

; Matcher<TypeLoc> Inner -> Matcher<CXXNewExpr>
(define (hasTypeLoc TypeLocMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CXXOperatorCallExpr>
(define (hasEitherOperand ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CXXOperatorCallExpr>
(define (hasLHS ExprMatchers) todo)

; Matcher<Expr> Matcher1, Matcher<Expr> Matcher2 -> Matcher<CXXOperatorCallExpr>
(define (hasOperands Expr> Matcher1 ,Matcher<ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CXXOperatorCallExpr>
(define (hasRHS ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CXXOperatorCallExpr>
(define (hasUnaryOperand ExprMatchers) todo)

; Matcher<CXXBaseSpecifier> BaseSpecMatcher -> Matcher<CXXRecordDecl>
(define (hasAnyBase CXXBaseSpecifierMatchers) todo)

; Matcher<CXXBaseSpecifier> BaseSpecMatcher -> Matcher<CXXRecordDecl>
(define (hasDirectBase CXXBaseSpecifierMatchers) todo)

; Matcher<CXXMethodDecl> InnerMatcher -> Matcher<CXXRecordDecl>
(define (hasMethod CXXMethodDeclMatchers) todo)

; Matcher<NamedDecl> Base -> Matcher<CXXRecordDecl>
(define (isDerivedFrom NamedDeclMatchers) todo)

; Matcher<NamedDecl> Base -> Matcher<CXXRecordDecl>
(define (isDirectlyDerivedFrom NamedDeclMatchers) todo)

; Matcher<NamedDecl> Base -> Matcher<CXXRecordDecl>
(define (isSameOrDerivedFrom NamedDeclMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CXXRewrittenBinaryOperator>
(define (hasEitherOperand ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CXXRewrittenBinaryOperator>
(define (hasLHS ExprMatchers) todo)

; Matcher<Expr> Matcher1, Matcher<Expr> Matcher2 -> Matcher<CXXRewrittenBinaryOperator>
(define (hasOperands Expr> Matcher1 ,Matcher<ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CXXRewrittenBinaryOperator>
(define (hasRHS ExprMatchers) todo)

; Matcher<TypeLoc> Inner -> Matcher<CXXTemporaryObjectExpr>
(define (hasTypeLoc TypeLocMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CXXUnresolvedConstructExpr>
(define (hasAnyArgument ExprMatchers) todo)

; unsigned N, Matcher<Expr> InnerMatcher -> Matcher<CXXUnresolvedConstructExpr>
(define (hasArgument unsignedParams) todo)

; Matcher<TypeLoc> Inner -> Matcher<CXXUnresolvedConstructExpr>
(define (hasTypeLoc TypeLocMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<CallExpr>
(define (callee DeclMatchers) todo)

; Matcher<Stmt> InnerMatcher -> Matcher<CallExpr>
(define (callee StmtMatchers) todo)

; Matcher<Expr> ArgMatcher, Matcher<ParmVarDecl> ParamMatcher -> Matcher<CallExpr>
(define (forEachArgumentWithParam Expr> ArgMatcher ,Matcher<ParmVarDeclMatchers) todo)

; Matcher<Expr> ArgMatcher, Matcher<QualType> ParamMatcher -> Matcher<CallExpr>
(define (forEachArgumentWithParamType Expr> ArgMatcher ,Matcher<QualTypeMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CallExpr>
(define (hasAnyArgument ExprMatchers) todo)

; unsigned N, Matcher<Expr> InnerMatcher -> Matcher<CallExpr>
(define (hasArgument unsignedParams) todo)

; Matcher<Decl> InnerMatcher -> Matcher<CallExpr>
(define (hasDeclaration DeclMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CaseStmt>
(define (hasCaseConstant ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<CastExpr>
(define (hasSourceExpression ExprMatchers) todo)

; clang::ast_matchers::Matcher<TemplateArgument> InnerMatcher -> Matcher<ClassTemplateSpecializationDecl>
(define (forEachTemplateArgument clangParams) todo)

; Matcher<TemplateArgument> InnerMatcher -> Matcher<ClassTemplateSpecializationDecl>
(define (hasAnyTemplateArgument TemplateArgumentMatchers) todo)

; Matcher<ClassTemplateDecl> InnerMatcher -> Matcher<ClassTemplateSpecializationDecl>
(define (hasSpecializedTemplate ClassTemplateDeclMatchers) todo)

; unsigned N, Matcher<TemplateArgument> InnerMatcher -> Matcher<ClassTemplateSpecializationDecl>
(define (hasTemplateArgument unsignedParams) todo)

; Matcher<TypeLoc> Inner -> Matcher<ClassTemplateSpecializationDecl>
(define (hasTypeLoc TypeLocMatchers) todo)

; Matcher<Type> -> Matcher<ComplexType>
(define (hasElementType MatcherParams) todo)

; Matcher<TypeLoc> Inner -> Matcher<CompoundLiteralExpr>
(define (hasTypeLoc TypeLocMatchers) todo)

; Matcher<Stmt> InnerMatcher -> Matcher<CompoundStmt>
(define (hasAnySubstatement StmtMatchers) todo)

; Matcher<QualType> InnerType -> Matcher<DecayedType>
(define (hasDecayedType QualTypeMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<DeclRefExpr>
(define (hasDeclaration DeclMatchers) todo)

; unsigned Index, Matcher<TemplateArgumentLoc> InnerMatcher -> Matcher<DeclRefExpr>
(define (hasTemplateArgumentLoc unsignedParams) todo)

; Matcher<UsingShadowDecl> Inner -> Matcher<DeclRefExpr>
(define (throughUsingDecl UsingShadowDeclMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<DeclRefExpr>
(define (to DeclMatchers) todo)

; unsigned N, Matcher<Decl> InnerMatcher -> Matcher<DeclStmt>
(define (containsDeclaration unsignedParams) todo)

; Matcher<Decl> InnerMatcher -> Matcher<DeclStmt>
(define (hasSingleDecl DeclMatchers) todo)

; Matcher<TypeLoc> Inner -> Matcher<DeclaratorDecl>
(define (hasTypeLoc TypeLocMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<Decl>
(define (hasDeclContext DeclMatchers) todo)

; Matcher<Type> -> Matcher<DecltypeType>
(define (hasUnderlyingType MatcherParams) todo)

; Matcher<BindingDecl> InnerMatcher -> Matcher<DecompositionDecl>
(define (hasAnyBinding BindingDeclMatchers) todo)

; unsigned N, Matcher<BindingDecl> InnerMatcher -> Matcher<DecompositionDecl>
(define (hasBinding unsignedParams) todo)

; Matcher<Stmt> InnerMatcher -> Matcher<DoStmt>
(define (hasBody StmtMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<DoStmt>
(define (hasCondition ExprMatchers) todo)

; Matcher<TypeLoc> InnerMatcher -> Matcher<ElaboratedTypeLoc>
(define (hasNamedTypeLoc TypeLocMatchers) todo)

; Matcher<NestedNameSpecifier> InnerMatcher -> Matcher<ElaboratedType>
(define (hasQualifier NestedNameSpecifierMatchers) todo)

; Matcher<QualType> InnerMatcher -> Matcher<ElaboratedType>
(define (namesType QualTypeMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<EnumType>
(define (hasDeclaration DeclMatchers) todo)

; Matcher<QualType> InnerMatcher -> Matcher<ExplicitCastExpr>
(define (hasDestinationType QualTypeMatchers) todo)

; Matcher<TypeLoc> Inner -> Matcher<ExplicitCastExpr>
(define (hasTypeLoc TypeLocMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<Expr>
(define (hasType DeclMatchers) todo)

; Matcher<QualType> InnerMatcher -> Matcher<Expr>
(define (hasType QualTypeMatchers) todo)

; ast_matchers::Matcher<Expr> InnerMatcher -> Matcher<Expr>
(define (ignoringElidableConstructorCall ast_matchersParams) todo)

; Matcher<Expr> InnerMatcher -> Matcher<Expr>
(define (ignoringImpCasts ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<Expr>
(define (ignoringImplicit ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<Expr>
(define (ignoringParenCasts ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<Expr>
(define (ignoringParenImpCasts ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<Expr>
(define (ignoringParens ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<FieldDecl>
(define (hasInClassInitializer ExprMatchers) todo)

; Matcher<Stmt> InnerMatcher -> Matcher<ForStmt>
(define (hasBody StmtMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<ForStmt>
(define (hasCondition ExprMatchers) todo)

; Matcher<Stmt> InnerMatcher -> Matcher<ForStmt>
(define (hasIncrement StmtMatchers) todo)

; Matcher<Stmt> InnerMatcher -> Matcher<ForStmt>
(define (hasLoopInit StmtMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<FriendDecl>
(define (hasType DeclMatchers) todo)

; Matcher<QualType> InnerMatcher -> Matcher<FriendDecl>
(define (hasType QualTypeMatchers) todo)

; clang::ast_matchers::Matcher<TemplateArgument> InnerMatcher -> Matcher<FunctionDecl>
(define (forEachTemplateArgument clangParams) todo)

; Matcher<Stmt> InnerMatcher -> Matcher<FunctionDecl>
(define (hasAnyBody StmtMatchers) todo)

; Matcher<ParmVarDecl> InnerMatcher -> Matcher<FunctionDecl>
(define (hasAnyParameter ParmVarDeclMatchers) todo)

; Matcher<TemplateArgument> InnerMatcher -> Matcher<FunctionDecl>
(define (hasAnyTemplateArgument TemplateArgumentMatchers) todo)

; Matcher<Stmt> InnerMatcher -> Matcher<FunctionDecl>
(define (hasBody StmtMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<FunctionDecl>
(define (hasExplicitSpecifier ExprMatchers) todo)

; unsigned N, Matcher<ParmVarDecl> InnerMatcher -> Matcher<FunctionDecl>
(define (hasParameter unsignedParams) todo)

; Matcher<TypeLoc> ReturnMatcher -> Matcher<FunctionDecl>
(define (hasReturnTypeLoc TypeLocMatchers) todo)

; unsigned N, Matcher<TemplateArgument> InnerMatcher -> Matcher<FunctionDecl>
(define (hasTemplateArgument unsignedParams) todo)

; Matcher<QualType> InnerMatcher -> Matcher<FunctionDecl>
(define (returns QualTypeMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<IfStmt>
(define (hasCondition ExprMatchers) todo)

; Matcher<DeclStmt> InnerMatcher -> Matcher<IfStmt>
(define (hasConditionVariableStatement DeclStmtMatchers) todo)

; Matcher<Stmt> InnerMatcher -> Matcher<IfStmt>
(define (hasElse StmtMatchers) todo)

; Matcher<Stmt> InnerMatcher -> Matcher<IfStmt>
(define (hasInitStatement StmtMatchers) todo)

; Matcher<Stmt> InnerMatcher -> Matcher<IfStmt>
(define (hasThen StmtMatchers) todo)

; Matcher<QualType> InnerMatcher -> Matcher<ImplicitCastExpr>
(define (hasImplicitDestinationType QualTypeMatchers) todo)

; unsigned N, ast_matchers::Matcher<Expr> InnerMatcher -> Matcher<InitListExpr>
(define (hasInit unsignedParams) todo)

; Matcher<Expr> InnerMatcher -> Matcher<InitListExpr>
(define (hasSyntacticForm ExprMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<InjectedClassNameType>
(define (hasDeclaration DeclMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<LabelStmt>
(define (hasDeclaration DeclMatchers) todo)

; Matcher<ValueDecl> InnerMatcher -> Matcher<LambdaCapture>
(define (capturesVar ValueDeclMatchers) todo)

; Matcher<LambdaCapture> InnerMatcher -> Matcher<LambdaExpr>
(define (forEachLambdaCapture LambdaCaptureMatchers) todo)

; Matcher<LambdaCapture> InnerMatcher -> Matcher<LambdaExpr>
(define (hasAnyCapture LambdaCaptureMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<MemberExpr>
(define (hasDeclaration DeclMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<MemberExpr>
(define (hasObjectExpression ExprMatchers) todo)

; Matcher<ValueDecl> InnerMatcher -> Matcher<MemberExpr>
(define (member ValueDeclMatchers) todo)

; Matcher<Type> -> Matcher<MemberPointerType>
(define (pointee MatcherParams) todo)

; Matcher<NamedDecl> InnerMatcher -> Matcher<NamedDecl>
(define (hasUnderlyingDecl NamedDeclMatchers) todo)

; Matcher<NestedNameSpecifierLoc> InnerMatcher -> Matcher<NestedNameSpecifierLoc>
(define (hasPrefix NestedNameSpecifierLocMatchers) todo)

; Matcher<NestedNameSpecifier> InnerMatcher -> Matcher<NestedNameSpecifierLoc>
(define (loc NestedNameSpecifierMatchers) todo)

; Matcher<TypeLoc> InnerMatcher -> Matcher<NestedNameSpecifierLoc>
(define (specifiesTypeLoc TypeLocMatchers) todo)

; Matcher<NestedNameSpecifier> InnerMatcher -> Matcher<NestedNameSpecifier>
(define (hasPrefix NestedNameSpecifierMatchers) todo)

; Matcher<NamespaceDecl> InnerMatcher -> Matcher<NestedNameSpecifier>
(define (specifiesNamespace NamespaceDeclMatchers) todo)

; Matcher<QualType> InnerMatcher -> Matcher<NestedNameSpecifier>
(define (specifiesType QualTypeMatchers) todo)

; Matcher<OMPClause> InnerMatcher -> Matcher<OMPExecutableDirective>
(define (hasAnyClause OMPClauseMatchers) todo)

; Matcher<Stmt> InnerMatcher -> Matcher<OMPExecutableDirective>
(define (hasStructuredBlock StmtMatchers) todo)

; Matcher<NamedDecl> Base -> Matcher<ObjCInterfaceDecl>
(define (isDerivedFrom NamedDeclMatchers) todo)

; Matcher<NamedDecl> Base -> Matcher<ObjCInterfaceDecl>
(define (isDirectlyDerivedFrom NamedDeclMatchers) todo)

; Matcher<NamedDecl> Base -> Matcher<ObjCInterfaceDecl>
(define (isSameOrDerivedFrom NamedDeclMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<ObjCMessageExpr>
(define (callee DeclMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<ObjCMessageExpr>
(define (hasAnyArgument ExprMatchers) todo)

; unsigned N, Matcher<Expr> InnerMatcher -> Matcher<ObjCMessageExpr>
(define (hasArgument unsignedParams) todo)

; Matcher<Expr> InnerMatcher -> Matcher<ObjCMessageExpr>
(define (hasReceiver ExprMatchers) todo)

; Matcher<QualType> InnerMatcher -> Matcher<ObjCMessageExpr>
(define (hasReceiverType QualTypeMatchers) todo)

; Matcher<ParmVarDecl> InnerMatcher -> Matcher<ObjCMethodDecl>
(define (hasAnyParameter ParmVarDeclMatchers) todo)

; unsigned N, Matcher<ParmVarDecl> InnerMatcher -> Matcher<ObjCMethodDecl>
(define (hasParameter unsignedParams) todo)

; Matcher<TypeLoc> Inner -> Matcher<ObjCPropertyDecl>
(define (hasTypeLoc TypeLocMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<OpaqueValueExpr>
(define (hasSourceExpression ExprMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<OverloadExpr>
(define (hasAnyDeclaration DeclMatchers) todo)

; Matcher<Type> -> Matcher<ParenType>
(define (innerType MatcherParams) todo)

; Matcher<TypeLoc> PointeeMatcher -> Matcher<PointerTypeLoc>
(define (hasPointeeLoc TypeLocMatchers) todo)

; Matcher<Type> -> Matcher<PointerType>
(define (pointee MatcherParams) todo)

; Matcher<QualType> InnerMatcher -> Matcher<QualType>
(define (hasCanonicalType QualTypeMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<QualType>
(define (hasDeclaration DeclMatchers) todo)

; Matcher<QualType> InnerMatcher -> Matcher<QualType>
(define (ignoringParens QualTypeMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<QualType>
(define (pointsTo DeclMatchers) todo)

; Matcher<QualType> InnerMatcher -> Matcher<QualType>
(define (pointsTo QualTypeMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<QualType>
(define (references DeclMatchers) todo)

; Matcher<QualType> InnerMatcher -> Matcher<QualType>
(define (references QualTypeMatchers) todo)

; Matcher<TypeLoc> InnerMatcher -> Matcher<QualifiedTypeLoc>
(define (hasUnqualifiedLoc TypeLocMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<RecordType>
(define (hasDeclaration DeclMatchers) todo)

; Matcher<TypeLoc> ReferentMatcher -> Matcher<ReferenceTypeLoc>
(define (hasReferentLoc TypeLocMatchers) todo)

; Matcher<Type> -> Matcher<ReferenceType>
(define (pointee MatcherParams) todo)

; Matcher<Expr> InnerMatcher -> Matcher<ReturnStmt>
(define (hasReturnValue ExprMatchers) todo)

; Matcher<Stmt> InnerMatcher -> Matcher<StmtExpr>
(define (hasAnySubstatement StmtMatchers) todo)

; Matcher<UnaryExprOrTypeTraitExpr> InnerMatcher -> Matcher<Stmt>
(define (alignOfExpr UnaryExprOrTypeTraitExprMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<Stmt>
(define (forCallable DeclMatchers) todo)

; Matcher<FunctionDecl> InnerMatcher -> Matcher<Stmt>
(define (forFunction FunctionDeclMatchers) todo)

; Matcher<UnaryExprOrTypeTraitExpr> InnerMatcher -> Matcher<Stmt>
(define (sizeOfExpr UnaryExprOrTypeTraitExprMatchers) todo)

; Matcher<Type> -> Matcher<SubstTemplateTypeParmType>
(define (hasReplacementType MatcherParams) todo)

; Matcher<SwitchCase> InnerMatcher -> Matcher<SwitchStmt>
(define (forEachSwitchCase SwitchCaseMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<SwitchStmt>
(define (hasCondition ExprMatchers) todo)

; Matcher<Stmt> InnerMatcher -> Matcher<SwitchStmt>
(define (hasInitStatement StmtMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<TagType>
(define (hasDeclaration DeclMatchers) todo)

; Matcher<TypeLoc> Inner -> Matcher<TemplateArgumentLoc>
(define (hasTypeLoc TypeLocMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<TemplateArgument>
(define (isExpr ExprMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<TemplateArgument>
(define (refersToDeclaration DeclMatchers) todo)

; Matcher<QualType> InnerMatcher -> Matcher<TemplateArgument>
(define (refersToIntegralType QualTypeMatchers) todo)

; Matcher<TemplateName> InnerMatcher -> Matcher<TemplateArgument>
(define (refersToTemplate TemplateNameMatchers) todo)

; Matcher<QualType> InnerMatcher -> Matcher<TemplateArgument>
(define (refersToType QualTypeMatchers) todo)

; Matcher<TemplateArgumentLoc> InnerMatcher -> Matcher<TemplateSpecializationTypeLoc>
(define (hasAnyTemplateArgumentLoc TemplateArgumentLocMatchers) todo)

; unsigned Index, Matcher<TemplateArgumentLoc> InnerMatcher -> Matcher<TemplateSpecializationTypeLoc>
(define (hasTemplateArgumentLoc unsignedParams) todo)

; clang::ast_matchers::Matcher<TemplateArgument> InnerMatcher -> Matcher<TemplateSpecializationType>
(define (forEachTemplateArgument clangParams) todo)

; Matcher<TemplateArgument> InnerMatcher -> Matcher<TemplateSpecializationType>
(define (hasAnyTemplateArgument TemplateArgumentMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<TemplateSpecializationType>
(define (hasDeclaration DeclMatchers) todo)

; unsigned N, Matcher<TemplateArgument> InnerMatcher -> Matcher<TemplateSpecializationType>
(define (hasTemplateArgument unsignedParams) todo)

; Matcher<Decl> InnerMatcher -> Matcher<TemplateTypeParmType>
(define (hasDeclaration DeclMatchers) todo)

; Matcher<QualType> InnerMatcher -> Matcher<TypeLoc>
(define (loc QualTypeMatchers) todo)

; Matcher<TypeLoc> Inner -> Matcher<TypedefNameDecl>
(define (hasTypeLoc TypeLocMatchers) todo)

; Matcher<QualType> InnerMatcher -> Matcher<TypedefNameDecl>
(define (hasType QualTypeMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<TypedefType>
(define (hasDeclaration DeclMatchers) todo)

; Matcher<Type> InnerMatcher -> Matcher<Type>
(define (hasUnqualifiedDesugaredType TypeMatchers) todo)

; Matcher<QualType> InnerMatcher -> Matcher<UnaryExprOrTypeTraitExpr>
(define (hasArgumentOfType QualTypeMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<UnaryOperator>
(define (hasUnaryOperand ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<UnresolvedMemberExpr>
(define (hasObjectExpression ExprMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<UnresolvedUsingType>
(define (hasDeclaration DeclMatchers) todo)

; Matcher<NamedDecl> InnerMatcher -> Matcher<UsingShadowDecl>
(define (hasTargetDecl NamedDeclMatchers) todo)

; Matcher<Type> -> Matcher<UsingType>
(define (hasUnderlyingType MatcherParams) todo)

; Matcher<UsingShadowDecl> Inner -> Matcher<UsingType>
(define (throughUsingDecl UsingShadowDeclMatchers) todo)

; Matcher<Decl> InnerMatcher -> Matcher<ValueDecl>
(define (hasType DeclMatchers) todo)

; Matcher<QualType> InnerMatcher -> Matcher<ValueDecl>
(define (hasType QualTypeMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<VarDecl>
(define (hasInitializer ExprMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<VariableArrayType>
(define (hasSizeExpr ExprMatchers) todo)

; Matcher<Stmt> InnerMatcher -> Matcher<WhileStmt>
(define (hasBody StmtMatchers) todo)

; Matcher<Expr> InnerMatcher -> Matcher<WhileStmt>
(define (hasCondition ExprMatchers) todo)
